#include "game.hpp"
#include "title.hpp"
#include <cstdio>
#include <cstdlib>


bool checkCommandArg(int argc, char *argv[], int &height, int &width, int &mine_num);


bool checkCommandArg(int argc, char *argv[], int &height, int &width, int &mine_num){
    if(argc != 1 && argc != 4) return false;
    
    if(argc == 4){
        for(int i = 1; i <= 3; i++){
            if(argv[i][0] != '-'){
                return false;
            }
            
            int num = 0;
            for(char* s = argv[i] + 1; *s != '\0'; s++){
                if('0' <= *s && *s <= '9'){
                    num *= 10;
                    num += *s - '0';
                }
                else{
                    return false;
                }
            }
            
            switch (i) {
            case 1:
                height = num;
                break;
            case 2:
                width = num;
                break;
            case 3:
                mine_num = num;
                break;
            default:
                break;
            }
        }
    }
    
    return true;
}


int main(int argc, char *argv[]){
    int height = 10, width = 10, mine_num = 10; // 縦幅、横幅、地雷の数の初期値
    
    if(!checkCommandArg(argc, argv, height, width, mine_num)){
        fprintf(stderr, "引数が不正です。\n");
        exit(1);
    }
    
    viewTitle(height, width, mine_num);
}