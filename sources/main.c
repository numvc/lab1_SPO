#include "../headers/user_interface.h"
#include "../headers/mode.h"
#include "../headers/partition.h"
#include "../headers/output_util.h"

int main(int argc, char* argv[]) {
    struct program args = parse_args(argc, argv);
    if (args.mode == UNDEFINED){
        printf("Incorrect arguments of the programme.\n");
        printf("\t-m - mode of the programme. 1 - Partitions. 2 - EXT3 filesystem.\n");
        printf("\t-p - path to the device.\n");
    }else if (args.mode == PARTITIONS) {
        struct output_for_partition* result = get_partitions();
        print_partitions(result);
    }else if (args.mode == EXT3){
        if (entrance(args.path) == 1) printf("Произошла ошибка.\n");
    }
    return 0;
}
