#include <cassert>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <vector>
#include <map>

#include "poolalloc.h"
#include "mylist.h"

template <typename T>
void show_map(const T& m ){
//    std::cout << __PRETTY_FUNCTION__ << std::endl;
    for(const auto &elem: m){
        std::cout << elem.first << ' ' << elem.second << std::endl;
    }
}

int main(int, char **)
{
    try
    {
        // создание экземпляра std::map<int, int>
   	std::map<int, int > m;

        // заполнение 10 элементами, где ключ - это число от 0 до 9, а значение - факториал ключа
    	for(auto i = 0; i < 10; ++i){
            if( i > 1){
                 m[i] = m[i-1] * i;
            } else{
                 m[i] = 1;
            }
        }

        //show_map(m);

        // создание экземпляра std::map<int, int> с новым аллокатором, ограниченным 10 элементами
        std::map<int, int, std::less<int>, pool_allocator<std::pair<const int, int>, 10 > > ma;

        // заполнение 10 элементами, где ключ - это число от 0 до 9, а значение - факториал ключа
        for(const auto &elem: m){
  	    ma[elem.first] = elem.second;	                                                         
        }

        // вывод на экран всех значений (ключ и значение разделены пробелом) хранящихся в контейнере
        show_map(ma);
	
        // создание экземпляра своего контейнера для хранения значений типа int
        my_list<int> list;

        // заполнение 10 элементами от 0 до 9
    	for(int i = 0; i < 10; ++i){
             list.append(i);
        }

        //  создание экземпляра своего контейнера для хранения значений типа int с новым аллокатором, ограниченным 10 элементами
        my_list<int, pool_allocator<int,10> > lista;

        // заполнение 10 элементами от 0 до 9
    	for(int i = 0; i < 10; ++i){
             lista.append(i);
        }
	
        //  вывод на экран всех значений, хранящихся в контейнере
    	for (auto it = list.begin(); it != list.end(); ++it) {
       	    std::cout << *it << std::endl;
    	}

    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }


    return 0;
}
