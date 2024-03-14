# Implementarea unui Arbore Binar de Căutare (BST) Echilibrat

Codul C prezentat implementează un arbore binar de căutare (BST) echilibrat pentru gestionarea unor structuri de date care reprezintă depozite, având informații precum denumire, locație, număr de angajați, salarii și vechime. La punctele marcate în funcția main, se realizează următoarele operații:

## Crearea și Inserarea în BST:
- Se citește dintr-un fișier și se creează structuri `DepozitInfo` care sunt apoi inserate în arborele binar de căutare.

## Vectorul de Depozite de la un Anumit Nivel:
- Se construiește un vector care conține toate depozitele de la un anumit nivel al arborelui (nivelul 2 în exemplul dat) și se afișează acest vector.

## Vectorul Drumului pentru un Depozit Specific:
- Se generează și se afișează un vector care conține toate nodurile (depozitele) de pe calea către un nod cu o anumită cheie (în acest caz, un anumit număr de angajați).

## Ștergerea Nodurilor-Frunză:
- Se sterg toate nodurile frunză din arbore.

## Dezalocarea Memoriei:
- Se eliberează toată memoria alocată pentru structurile de date din program, inclusiv arborele și vectorii creați.

## Afisare După Dezalocare:
- Se afiseaza structurile dupa dezalocare pentru verificare.
