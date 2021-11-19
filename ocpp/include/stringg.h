#ifndef STRING_H_

#define STRING_H_


void
strcpyy
(
    char *_dest,
    char *_src
);

unsigned int
strlenn
(
    char *_str
);

void
erase
(
    char *_str,
    int value,
    unsigned int size
);

#endif /* STRING_H_ */