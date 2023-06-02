#include <iostream>
#include <string.h>
#include "make.h"


int main(int argc, char *argv[])
{
    if(argc > 3)
    {
        std::puts("Invalid number of arguments ... program terminated.");
        return EXIT_FAILURE;
    }

    if(strcmp(argv[0], "make"))
    {
        return EXIT_FAILURE;
    }

    std::string map_dir = create_map_dir();
    const char *dirPtr = map_dir.c_str();

    /* Check map's directory */
    check_map_dir(dirPtr);

    map_dir.append("/map.txt");
    dirPtr = map_dir.c_str();

    /* Opening the map file */
    FILE *map_file = check_map_file(dirPtr);
    if(map_file == nullptr)
    {
        return EXIT_FAILURE; 
    }
    
    /* Checking user's arguments */
    if(argc == 1)
    {
        std::printf("%s\n", "Missing option ... program terminated."); 
        return EXIT_FAILURE;
    }
    else if(argc == 2)
    {
        if(!(!strcmp(argv[1], "c")    || !strcmp(argv[1], "cpp") || !strcmp(argv[1], "xc8")   ||
             !strcmp(argv[1], "rust") || !strcmp(argv[1], "zig") || !strcmp(argv[1], "java")
          ) )
        {
            std::printf("Invalid option ==> (%s) ... program terminated.\n", argv[1]); 
            return EXIT_FAILURE;   
        }
    }
    else if(argc == 3)
    {
        if(!(!strcmp(argv[1], "c")    || !strcmp(argv[1], "cpp") || !strcmp(argv[1], "xc8")   ||
             !strcmp(argv[1], "rust") || !strcmp(argv[1], "zig") || !strcmp(argv[1], "java")
          ) )
        {
            if(!strcmp(argv[2], "-map") || !strcmp(argv[2], "map"))
            {
                /* Open the map file in the code editor specified by user */
                open_map_file(argv[1], &map_file, dirPtr);  
                return EXIT_SUCCESS;
            }
            else if(!strcmp(argv[2], "-print"))
            {
                std::printf("Invalid option ==> (%s) ... program terminated.\n", argv[1]); 
                return EXIT_FAILURE;
            }
            else if(strcmp(argv[2], "-print"))
            {
                std::printf("Invalid option ==> (%s) ... program terminated.\n", argv[2]);  
                return EXIT_FAILURE;
            }
        }
        else
        {
            if(strcmp(argv[2], "-print"))
            {
                std::printf("Invalid option ==> (%s) ... program terminated.\n", argv[2]);   
                return EXIT_FAILURE;
            }
        }
    }


    std::string map_content = {};

    /* Get the content and the size of the map file */
    std::size_t content_size = get_map_file(map_content, &map_file); 

    clean_up(&map_file);

    /* Get the desired tokens if possible */
    std::string tokens = get_tokens(&map_content.front(), content_size);
     
    /* Update content size */
    content_size = tokens.size(); 
     
    std::string cmd = {}; 
    char *content = (char *) &tokens.front(); 
    int line = 0;
    
    /**                                                    **
     * Checking the basic textual structure of the map file *
     **                                                    **/ 
    
    if(tokens.empty())
    {
        std::printf("Error: map.txt should begins with \"makeMap{...., ..., etc}\"\n%s\n",
                    "The basic textual structure of map.txt can't be changed.\n"
                    "Fix this problem or remove the current map.txt and run the program again\n"
                    "in order to let the program to create A new map.txt in the map directory.\n"
                    "You may open the New map.txt to complete the configurations.\n"
                    "program terminated.");
        return EXIT_FAILURE;
    }

    std::size_t k = 7;

    if(!(content[k] == '{' || content[k+1] == '{'))
    {
        do
        {
            k++;
        }
        while (content[k] == '\n');
        if(content[k] != '{')
        {
            std::printf("Error: missing left parentheses '{' before the configurations list ...\n%s\n",
                        "The basic textual structure of map.txt can't be changed.\n"
                        "Fix this problem or remove the current map.txt and run the program again\n"
                        "in order to let the program to create A new map.txt in the map directory.\n"
                        "You may open the New map.txt to complete the configurations.\n"
                        "program terminated.");
            return EXIT_FAILURE; 
        }
    }
    
    if(content[content_size-1] != '}')
    {
        do
        {
            content_size--;
        }
        while (content[content_size] == '\n');
        
        if(content[content_size] != '}')
        {
            std::printf("Error: missing right parentheses '}' after the configurations list ...\n%s\n",
                        "The basic textual structure of map.txt can't be changed.\n"
                        "Fix this problem or remove the current map.txt and run the program again\n"
                        "in order to let the program to create A new map.txt in the map directory.\n"
                        "You may open the New map.txt to complete the configurations.\n"
                        "program terminated.");
            return EXIT_FAILURE; 
        }
    }

    
    /* Going through the map file */
    for (std::size_t i = 0; i < content_size; ++i)
    {

        if(content[i] == '\n')
        {
            ++line;
        }

        if((!strncmp(&content[i], "\"c\"", 3)     &&  !strcmp(argv[1], "c"))      ||
           (!strncmp(&content[i], "\"cpp\"", 5)   &&  !strcmp(argv[1], "cpp"))    ||
           (!strncmp(&content[i], "\"xc8\"", 5)   &&  !strcmp(argv[1], "xc8"))    ||
           (!strncmp(&content[i], "\"rust\"", 6)  &&  !strcmp(argv[1], "rust"))   ||
           (!strncmp(&content[i], "\"zig\"", 5)   &&  !strcmp(argv[1], "zig"))    ||
           (!strncmp(&content[i], "\"java\"", 6)  &&  !strcmp(argv[1], "java")))
        {

            i += strlen(argv[1]) + 4;   
             
	   /**                                                    **
	    * Checking the basic textual structure of the map file *
            **                                                    **/ 
        
	    if(content[i-2] != ':')
            {
                std::printf("Error: expected colon ':' at line %i where '%c' is found ...\n%s\n", ++line, content[i-2],
                            "The basic textual structure of map.txt can't be changed.\n" 
                            "Fix this problem or remove the current map.txt and run the program again\n"
                            "in order to let the program to create A new map.txt in the map directory.\n"
                            "You may open the New map.txt to complete the configurations.\n"
                            "program terminated."); 
                std::exit(EXIT_FAILURE); 
            }

            if(content[i-1] != '"')
            {
                std::printf("Error: expected double quote '\"' at line %i where '%c' is found ...\n%s\n", ++line, content[i-1],
                            "The basic textual structure of map.txt can't be changed.\n"
                            "Fix this problem or remove the current map.txt and run the program again\n"
                            "in order to let the program to create A new map.txt in the map directory.\n"
                            "You may open the New map.txt to complete the configurations.\n"
                            "program terminated.");
                std::exit(EXIT_FAILURE);
            }

            /* Fetch user's configuration */
            for (; ; ++i)
            {
                if(content[i] == '\n')
                {
                    content[i] = ' '; 
                }
                if(content[i] == '\\' && content[i+1] == '"')
                {
                    cmd.push_back('"');
                    i++;
                    continue;
                }
                else if(content[i-1] != '\\' && content[i] == '"')
                {
                    break;
                }   
                cmd.push_back(content[i]); 
            }
            break;
        }   
    }

    /* Execute user's configuration */
    const char *cmd_ptr = cmd.c_str();
    if(argc == 2)
    {
        std::system(cmd_ptr);
    }
    else if(argc == 3)
    {
        std::printf("%s\n", cmd_ptr);
        std::system(cmd_ptr); 
    }
    else
    {
    }
    
    return EXIT_SUCCESS; 
}
