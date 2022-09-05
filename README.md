# frugal-lru-cache
Memory efficient LRU cache

### classic solution
Least Recently Used cache has three main operations:
- put new element on top of list
- remove element from end of list (if elements reaches limit)
- operation get() moves element from any place of list to the beginning of list

First operation can be available in any foraward list, but with second, list must be doubly linked.
Third operation requires random seek to any place of list. Must be implemented with map 
(map can be unordered). Map has key which is cache key, and value that is doubly linked list iterator.

### motivation
This has large memory overhead. The list is doubly linked, while in 64bit systems, pointers are 
wide.
To check memory usage, we use mallinfo2() which gives results to the struct mallinfo2.  
in malloc.h
```c
struct mallinfo2
{
..............
size_t hblkhd;   /* space in mmapped regions */
..............
size_t uordblks; /* total allocated space */
..............
};
```
The most important fields are hblkhd which is memory used in large blocks, and uordblks which is
memory used in small blocks
We can print difference
```cpp
void printDiff(struct mallinfo2 &ma, struct mallinfo2 &mb) {
    cout << double((mb.uordblks+mb.hblkhd)-(ma.uordblks+ma.hblkhd))/COUNT <<endl;
}
```
where 
```cpp
auto m1a =  mallinfo2();
............
auto m1B =  mallinfo2();
```
classic LRU cache uses near 76 bytes/element for cache<int,int>. 
It too much, especially if values stored in cache are small.

### proposal
We can discard completely list, leaving only map.
The unordered_map<in,int> has memory requirement near 44 bytes/element.
This costs somme fuzzing of cache expectations.
We have planar cache with map for new and old elements. If map for new elements will fill up, 
we leave old map, new map becomes old and init empty new map.
This guarantee keep N elements in cache and not guarantee up to 2N.
More predictable is to have more maps, for example 4 or 6, but the more maps, the more time
for get old elements, while get new elements still fast. Number of planes will be parameter.
Since 64 bit of pointers is too much, next proposal is resignation from pointers and 
instead of this use index in one large array.<br>
Array has elements named slots. Slot 0 is reserved because index 0 means here 'null pointer'.<br>
Map slot is:
```cpp
struct Slot {
    K key;
    V value;
    slot_t next;
};
```
In order not to waste space on the field indicating that the slot is in use,
we use bitmap. The bits in reverse 1 means empty slot, 0 means occupied slot,
to be able to use fast function ffsll, which search first least significant set bit in
64 bit word (similar ffs search 32 bit words).<br>
To search from specified position from and to, we AND the word with masks forms:
...00000011111111....  or ...1111111100000000...<br>
Whu bit position search?<br>
We construct hashmap on slots. First, we have key, we compute hash of the key, for example 
using 32bit MurmurHash from Wikipedia (https://en.wikipedia.org/wiki/MurmurHash), next obtain slot number 
using modulo and add 1 (because slot 0 is reserved).<br>
What if the slot is occupied? Usually, in memory hash maps, we will store
new pair<key, value> on top singly linked list (obviously still other operations like check
if key is in this list). Here we must search next empty slot and in first slot we set 'next'
field points to next field.<br>
Algorithm of 'Put' operation:
- if not occupied set new slot with key, value and set next to 0
- if occupied and key already exists in linked slot list - replace value
- if occupied and key not exists:
  - search new free slot
  - copy occupied slot N to new position
  - fill slot N with hist key and value
As example we show addings 12 elements to 12 position map. 
- The MurMurHash give following slots for keys from 0 to 11:
- 0 -> 11
- 1 -> 11
- 2 -> 12
- 3 -> 6
- 4 -> 8
- 5 -> 7
- 6 -> 11
- 7 -> 6
- 8 -> 6
- 9 -> 12
- 10 -> 6
- 11 -> 11

(for this hash 12 is small number with especially non-uniform
distribution)

```
star '*' means key match position

put key=0
not occupied set key 0 to slot 11
after is:
-11(*0) whole list: -11(*0)

put key=1
11 occupied, found new slot 12
before is:
-11(*0) whole list: -11(*0)
copy slot 11 to 12
and fill slot 11
after is:
-11(*1)-12(0) whole list: -11(*1)-12(0)

put key=2
12 occupied, found new slot 1
before is:
-12(0) whole list: -11(*1)-12(0)
copy slot 12 to 1
and fill slot 12
after is:
-12(*2)-1(0) whole list: -11(*1)-12(*2)-1(0)

put key=3
not occupied set key 3 to slot 6
after is:
-6(*3) whole list: -6(*3)

put key=4
not occupied set key 4 to slot 8
after is:
-8(*4) whole list: -8(*4)

put key=5
not occupied set key 5 to slot 7
after is:
-7(*5) whole list: -7(*5)

put key=6
11 occupied, found new slot 2
before is:
-11(*1)-12(*2)-1(0) whole list: -11(*1)-12(*2)-1(0)
copy slot 11 to 2
and fill slot 11
after is:
-11(*6)-2(1)-12(*2)-1(0) whole list: -11(*6)-2(1)-12(*2)-1(0)

put key=7
6 occupied, found new slot 9
before is:
-6(*3) whole list: -6(*3)
copy slot 6 to 9
and fill slot 6
after is:
-6(*7)-9(3) whole list: -6(*7)-9(3)

put key=8
6 occupied, found new slot 10
before is:
-6(*7)-9(3) whole list: -6(*7)-9(3)
copy slot 6 to 10
and fill slot 6
after is:
-6(*8)-10(7)-9(3) whole list: -6(*8)-10(7)-9(3)

put key=9
12 occupied, found new slot 3
before is:
-12(*2)-1(0) whole list: -11(*6)-2(1)-12(*2)-1(0)
copy slot 12 to 3
and fill slot 12
after is:
-12(*9)-3(2)-1(0) whole list: -11(*6)-2(1)-12(*9)-3(2)-1(0)

put key=10
6 occupied, found new slot 4
before is:
-6(*8)-10(7)-9(3) whole list: -6(*8)-10(7)-9(3)
copy slot 6 to 4
and fill slot 6
after is:
-6(*10)-4(8)-10(7)-9(3) whole list: -6(*10)-4(8)-10(7)-9(3)

put key=11
11 occupied, found new slot 5
before is:
-11(*6)-2(1)-12(*9)-3(2)-1(0) whole list: -11(*6)-2(1)-12(*9)-3(2)-1(0)
copy slot 11 to 5
and fill slot 11
after is:
-11(*11)-5(6)-2(1)-12(*9)-3(2)-1(0) whole list: -11(*11)-5(6)-2(1)-12(*9)-3(2)-1(0)
```

#### erasing
Erasing is problematic, because one slot can belong to
muliple lists. Instead of erasing is set bit "erased' in another bitmap,
and slot is busy until map garbage collector rewrite buffer to
new buffer the same size or greater size.<br>
Planar cache don't need erase single elements, instead of this release 
whole old map.<br>
This way slot map<int,int> uses only 12.25 bytes per element,
(0.25 = 2 bits of two bitmaps
