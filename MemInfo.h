//
// Created by Andrzej Borucki on 2022-09-01
//

#ifndef GTEST_FLRU_MEMINFO_H
#define GTEST_FLRU_MEMINFO_H


class MemInfo {
    int parseLine(char *line);
public:
    int virtMem();
    int resMem();
};


#endif //GTEST_FLRU_MEMINFO_H
