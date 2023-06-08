#ifndef MAKE_H
#define MAKE_H

extern void check_map_dir(const char *__dirPtr); 

extern FILE *check_map_file(const char *__dirPtr);

extern std::string create_map_dir(void);

extern void open_map_file(const char *argv1, FILE **map, const char *__dirPtr);

extern void clean_up(FILE **map);

extern std::size_t get_map_file(std::string &_file_content, FILE **map);

extern std::string get_tokens(const char *map_content, std::size_t map_size);

#endif
