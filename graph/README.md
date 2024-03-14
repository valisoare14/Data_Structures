# Implementare Graf Neorientat în C

Acest fișier C implementează o structură de date de tip graf neorientat folosind liste de adiacență și apoi convertește această structură într-o matrice de adiacență.

## Principalele componente și funcțiile lor:

### Structuri de Date:
- **Neighbour**: Reprezintă vecinii unui nod în graf (lista de adiacență).
- **Vertex**: Reprezintă nodurile grafului și conține o listă de vecini (Neighbour).
- **Graph**: Un pointer la primul Vertex din graf, reprezentând însuși graful.

### Funcții de Gestionare a Memoriei:
- `createVertex()`: Creează un nou nod (Vertex) în graf.
- `createNeighbour()`: Creează un nou vecin (Neighbour) pentru un nod dat.

### Funcții de Manipulare a Grafului:
- `addEdge()`: Adaugă o nouă muchie în graf prin actualizarea listelor de adiacență pentru ambele noduri implicat în muchie.
- `searchVertex()`: Caută un nod în graf pe baza valorii sale.

## Funcții de Afișare:

- `printGraph()`: Afișează graful - nodurile și vecinii acestora.
- `printMatrix()`: Afișează matricea de adiacență a grafului.

## Conversia Graf în Matrice de Adiacență:

- `refineMatrix()`: Inițializează o matrice de adiacență cu toate elementele setate pe 0.
- `convertGraphToMatrix()`: Convertește structura grafică bazată pe liste de adiacență într-o matrice de adiacență, unde fiecare element `matrix[i][j]` este setat pe 1 dacă există o muchie între nodul `i+1` și nodul `j+1`.
