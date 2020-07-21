#include <iostream>
#include <vector>
#include <random>
#include <utility>
#include <list>
#include "gtest/gtest.h"

extern "C" {
#include "../libs/log.h"
#include "../libs/utils.h"
#include "../libs/kustd.h"
#include "../kernel/buddy.h"
#include "stub.h"
}

bool buddy_test_eq(BuddySystem b1, BuddySystem b2) {
    bool ret;
    ret = b1.buddy_system == b2.buddy_system &&
        b1.buddy_system_size == b1.buddy_system_size &&
        b1.managed == b2.managed &&
        b1.managed_page_num == b2.managed_page_num &&
        b1.managed_size == b2.managed_size;
    
    if(!ret) {
        std::cout << "ret == false\n";
        return false;
    }

    for(int i = 0; i < sizeof(b1.indices) / sizeof(b1.indices[0]); ++i) {
        auto k1 = b1.indices[i];
        auto k2 = b2.indices[i];

        while(k1 != -1 && k2 != -1) {
            if(k1 != k2 && b1.buddy_system[k1].pow == b2.buddy_system[k2].pow) {
                std::cout << "not eq1 " << k1 << ' ' << k2 << std::endl;
                return false;
            }
            k1 = b1.buddy_system[k1].next;
            k2 = b2.buddy_system[k2].next;
        }

        if(k1 != k2) {
            std::cout << "not eq2 " << k1 << ' ' << k2 << std::endl;
            return false;
        }
        
    }
    return true;

}



TEST(buddy_test, seq_test) {
    
    BuddySystem bs;
    std::vector<uint32_t> args1 = {
        0,
        1 << 10, 3 << 10
    };

    std::vector<uint32_t> args2 = {
        1 << 20, 3 << 20
    };

    uint32_t max_page_num = (7 << 20) / PAGE_SIZE + 1;

    std::vector<uint32_t> page_nums;


    for(int j = 1; j <= 5; ++j)
    for(int i = 1; i <= 5; ++i) {
        page_nums.push_back(i * j);
    }

    std::vector<void*> tmps;
    std::vector<BuddySystem> bss;

    BuddySystem bs_backup;
    
    void *buf = new uint8_t[max_page_num * sizeof(BuddyList) + 1];

    for(auto && arg1 : args1) {
        for(auto && arg2 : args2) {
            buddy_init(&bs, (void*)arg1, arg2, buf);
            buddy_debug_print(&bs);
            bs_backup = bs;
            std::cout << "Alloc\n";
            for(auto && alloc_num : page_nums) {
                bss.push_back(bs);
                auto tmp = buddy_alloc(&bs, alloc_num);
                buddy_debug_print(&bs);
                tmps.push_back(tmp);
            }

            std::cout << "Free\n";
            while(!tmps.empty()) {
                auto tmp = tmps.back();
                tmps.pop_back();
                buddy_free(&bs, tmp);
                buddy_debug_print(&bs);
                std::cout << '\n';
                bss.pop_back();
            }

            ASSERT_EQ(buddy_test_eq(bs_backup, bs), true);

            
        }
    }
}