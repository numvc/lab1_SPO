#include "../headers/mode.h"

const char* options = "m:p:";
struct program parse_args(const int argc, char** argv) {
    struct  program args;
    int opt;
    args.mode = UNDEFINED;
    while ((opt = getopt(argc, argv, options)) != -1) {
        switch (opt) {
            case 'm':
                if (strcmp(optarg, "=1") == 0) {
                    args.mode = PARTITIONS;
                }else if (strcmp(optarg, "=2") == 0){
                    args.mode = EXT3;
                }
                break;
            case 'p':
                args.path = optarg;
                args.path ++;
                break;
            default:
                printf("Неизвестный режим работы.\n");
                break;
        }
    }
    return args;
}