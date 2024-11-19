#ifndef ARRAY_H
#define ARRAY_H

#include "line.h"

% TEMPLATE_BEGIN % typedef struct {
    % TYPE % *items;
    int length;
    int capacity;
} % TYPE % _Array;

% TYPE % _Array* new % TYPE % Array();

void free % TYPE % Array(% TYPE % _Array * a);
% TEMPLATE_END %

#endif // ARRAY_H
