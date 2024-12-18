## Linear search

**Linear search** or **sequential search** is a method for finding an element within a list.
It sequentially checks each element of the list until a match is found or the whole list has been searched.

A linear search runs in linear time in the worst case, and makes at most n comparisons,
where n is the length of the list. If each element is equally likely to be searched,
then linear search has an average case of $\frac{n+1}{2}$ comparisons,
but the average case can be affected if the search probabilities for each element vary.
Linear search is rarely practical because other search algorithms and schemes,
such as the binary search algorithm and hash tables, allow significantly faster searching for all
but short lists. 