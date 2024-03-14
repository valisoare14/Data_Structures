# Implementarea unui Arbore Binare de Căutare pentru Informații despre Fotografii

Codul furnizat este un program în C care implementează un arbore binar de căutare (BinarySearchTree) pentru gestionarea informațiilor despre fotografii.

## Traversare Inordine:
Afișează toate fotografiile din arborele binar de căutare în ordine sortată. 
Traversarea inordine într-un BST afișează nodurile în ordinea crescătoare a cheilor, aici folosindu-se id-ul fotografiei ca și cheie.

## Căutare Fotografii în Același Oraș:
Contorizează câte fotografii sunt înregistrate într-un oraș specificat, în acest caz "Giurgiu". 
Funcția `searchForFotosInSameTown` parcurge arborele și numără nodurile care au orașul egal cu cel căutat.

## Modificare Dată Fotografie:
Schimbă data unei fotografii specificate prin id.
Funcția `modificareData` caută nodul cu id-ul specificat și actualizează data acelei fotografii.

## Dezalocare Rădăcină:
Elimină rădăcina arborelui binar de căutare și reajustează arborele astfel încât să rămână în continuare un BST valid. Funcția `dezalocareRadacina` gestionează acest proces.

## Conversie BST în Vector:
Transformă arborele binar de căutare într-un vector, adăugând în vector doar fotografiile cu o rezoluție specificată.
Funcțiile `fromBSTToVector` și `populateVector` sunt folosite pentru această conversie.

## Afișare Fotografii După O Anumită Dată:
Afișează toate fotografiile care au o dată mai recentă decât o dată specificată. 
Funcția `displayFotoOnlyAboveSpecifiedDate` compară datele și afișează doar fotografiile care îndeplinesc condiția.
