# Cobble regex patterns
    x - literal
    xy - concatenation
    x|y - union
    x* - closure
    x+ - positive closure
    (x) - group
    x{n} - repetition
    x{n,m} - repetition range
    x? - option
    \x - escape sequence
    [xyz] - character class
    [^xyz] - opposite character class
    . - wildcard - match anything but newline
    \w, \W - alphanumeric or _, opposite
    \s, \S - space, opposite
    \d, \D - digits, opposite
    \N - opposite of newline
    ^ - beginning of string
    $ - end of string