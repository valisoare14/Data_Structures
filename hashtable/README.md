# Manipularea Structurii de Date pentru Garanții de Produse

Codul C furnizat definește structuri și funcții pentru manipularea unei structuri de date de tip tabel de dispersie (hash table) care gestionează informații despre garanții de produse, precum și pentru conversia acestuia într-o listă dublu înlănțuită. În funcția main, se efectuează mai multe operațiuni, pe care le-am identificat cu comentariile de la punctele .1 până la .5:

1. **Încărcarea datelor în tabelul de dispersie:**
   - Se deschide un fișier text (Data.txt).
   - Pentru fiecare linie citită din acesta, se extrag detaliile garantiei.
   - Se creează o structură GarantieInfo.
   - Se populează tabelul de dispersie folosind aceste structuri de garantii.

2. **Căutarea în tabelul de dispersie:**
   - Se caută numărul de garanții asociate cu un anumit serviciu ("SecureGuard Insurance").
   - Se afișează acest număr.

3. **Extinderea Garanțiilor:**
   - Se parcurge tabelul hash.
   - Pentru fiecare garanție electronică (câmpul `esteElectronica` este adevărat/1), se extinde numărul de luni de garanție cu o valoare specificată (în acest caz, 7 luni).

4. **Modificarea Serviciului pentru o Garanție:**
   - Se schimbă numele serviciului pentru o anumită garanție.
   - Acest lucru presupune actualizarea informației din structura.
   - Posibil mutarea nodului în alt bucket dacă valoarea hash a noului serviciu diferă.

5. **Conversia Tabelului Hash în Listă Dublu Înlănțuită:**
   - Se convertește tabelul hash într-o listă dublu înlănțuită.
   - Se adaugă în listă doar garanțiile care au un număr de luni mai mic decât un prag specificat (în acest caz, 20 de luni).

