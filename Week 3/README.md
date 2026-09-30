## AVL strom

Skoro hotová implementace AVL stromu.
Zde stojí za to si dobře rozmyslet, že balancování je na správných místech, a že rotace jsou správně implementované.

Stojí za vyzkoušení co se stane, pokud do stromu budete vkládat typ, pro který definujete operátor `<` a `>`, tak, že nepovede na úplné uspořádání.
Například pokud budete vkládat do stromu `std::pair<int, int>`, a definujete `<` a `>` jen podle prvního prvku páru.
