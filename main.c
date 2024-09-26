/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 22:27:00 by mmravec           #+#    #+#             */
/*   Updated: 2024/09/26 17:11:54 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include "get_next_line.h"

// Function to simulate a read error (e.g., by closing the file descriptor)
void simulate_read_error(int fd)
{
    printf("Simulating read error...\n");
    close(fd); // Force close the file descriptor
}

int main(void)
{
    int     fd;
    char    *next_line;
    int     count;

    count = 0;

    printf("Reading using BUFFER_SIZE: %d\n", BUFFER_SIZE);
    // Open the test file (make sure read.txt exists and has content)
    fd = open("get_next_line.h", O_RDONLY);
    if (fd == -1)
    {
        printf("Error opening file\n");
        return (1);
    }

    // Print each line read from the file
    while (1)
    {
        next_line = get_next_line(fd);
        if (next_line == NULL)
            break;
        count++;
        printf("[%d]:%s\n", count, next_line); // Show line number and content
        free(next_line);
    }

    // Test 1: Simulate read error after a few lines
    simulate_read_error(fd);
    next_line = get_next_line(fd);  // This should fail due to the forced read error
    if (next_line == NULL)
    {
        printf("Read error handled correctly (NULL returned)\n");
    }
    else
    {
        printf("Error: Read error not handled correctly\n");
        free(next_line);  // Clean up if somehow allocated
    }

    // Reopen the file and read again to test static buffer reset
    fd = open("get_next_line.h", O_RDONLY);
    if (fd == -1)
    {
        printf("Error reopening file\n");
        return (1);
    }

    printf("\nRe-reading the file after resetting static buffer:\n");
    count = 0;
    while (1)
    {
        next_line = get_next_line(fd);
        if (next_line == NULL)
            break;
        count++;
        printf("[%d]:%s\n", count, next_line);
        free(next_line);
    }

    // Close the file descriptor
    close(fd);
    printf("Read using BUFFER_SIZE: %d\n", BUFFER_SIZE);
    // If using valgrind, you can include this line to trigger memory checking
    // system("valgrind --leak-check=full ./a.out");  // Uncomment if running valgrind

    return (0);
}
