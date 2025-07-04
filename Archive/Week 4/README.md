## Halda jako datová struktura
`main_heap.cpp` obsahuje přímočarou implementaci haldy. Součástí jsou dva postupy, které obnovují vlastnost haldy.
Jeden je implementovaný pomocí rekuzre jako funkce `heapify`, druhý je hlavní částí funkce `insert`, implementovaný pomocí cyklu.
Je fajn si rozmyslet, že stejně tak je možné `heapify` realizovat cycklem a `insert` rekurzí.
