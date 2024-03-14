# Sistem de Gestionare a Rezervărilor de Vagoane de Tren

În acest fișier C, se implementează un sistem de gestionare a rezervărilor de vagoane de tren, folosind o structură de date care combină o listă dublu înlănțuită și o coadă de priorități (priority queue).

## Afișare lista dublu înlănțuită:
- Se afișează lista de la cap la coadă și invers, prezentând detalii despre fiecare vagon, cum ar fi numărul vagonului, firma, numărul de bilete vândute și capacitatea.

## Ștergere vagon cu numărul minim de bilete vândute:
- Se elimină din listă vagonul cu cel mai mic număr de bilete vândute.

## Convertirea listei în coadă de priorități:
- Se transformă lista dublu înlănțuită într-o coadă de priorități, unde ordinea elementelor este determinată de un criteriu specific, precum numărul de bilete vândute sau un grad de ocupare.

## Modificare număr de bilete și afișare coadă de priorități:
- Se schimbă numărul de bilete vândute pentru un vagon specific și se afișează coada actualizată, care reflectă noua ordine de priorități.

## Dequeue (extragere) și afișare vagon extras:
- Se extrage vagonul cu cea mai mare prioritate din coadă (cel cu cel mai mic număr de bilete vândute sau cel mai mare grad de ocupare) și se afișează informațiile acestuia, urmate de afișarea cozii de priorități rămase după extragere.
