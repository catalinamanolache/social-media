**Echipa tema 3 SD:**
stefania.diaconescu
maria.manolache1203

**Social media:**
Un proiect de platforma de social media care utilizeaza grafuri si structuri
de date arborescente pentru gestionarea prietenilor, postarilor si repostarilor,
oferind functionalitati precum adaugarea si eliminarea prietenilor, sugestii de
prieteni, calcularea distantei intre utilizatori, gestionarea postarilor si
repostarilor si afisarea feed-ului utilizatorilor.

### Descriere:
Problema are ca scop implementarea unei platforme de social media cu
ajutorul grafurilor si structurilor de date arborescente.

Functiile de baza pentru prelucrarea listelor utilizate in program sunt:
* dll_create() - aloca o lista si initializeaza campurile acesteia
* dll_add_nth_node() - adauga un nod in lista pe pozitia n. Se aloca memorie
si sunt create legaturile in functie de locul in care este adaugat nodul.
* dll_remove_nth_node() - intoarce un pointer la nodul de pe pozitia n,
legaturile fiind desfacute
* dll_free() - dezaloca memoria ocupata de lista

Functiile de baza pentru prelucrarea cozii utilizate in program sunt:
* q_create() - aloca si initializeaza o coada
* q_get_size() - intoarce numarul de elemente din coada
* q_is_empty() - verifica daca este goala coada
* q_front() - intoarce primul element din coada, fara sa il elimine
* q_dequeue() - elimina ultimul element din coada
* q_clear() - elimina toate elementele din coada primita ca parametru
* q_free() - elibereaza memoria

Functiile de baza pentru prelucrarea grafurilor utilizate in program sunt:
* lg_create() - aloca si initializeza graful
* lg_add_edge() - adauga o muchie intre nodurile primite ca parametri
* lg_has_edge() - returneaza 1 daca exista muchie intre cele doua noduri, 0
in caz contrar
* lg_remove_edge() - elimina muchia dintre nodurile primite ca parametri
* lg_free() - elibereaza memoria folosita de lista de adiacenta a grafului
* bfs_list_graph() - functia returneaza distanta minima dintre doua noduri

### _Task-ul 1 - Retea de prietenie_

Functia 'handle_input_friends()' separa comenzile si apeleaza functiile
corespunzatoare.

- in cazul in care primul cuvant este 'add', este apelata functia
'add_friend()'. Se obtin id-urile utilizatorilor si este adaugata conexiunea
in ambele sensuri, la final fiind afisat mesajul corespunzator.

- in cazul in care primul cuvant este 'remove', este apelata functia
'remove_friend()'. Se obtin id-urile utilizatorilor si este stearsa conexiunea
din ambele sensuri, la final fiind afisat mesajul corespunzator.

- in cazul in care primul cuvant este 'suggestions', este apelata functia
'get_suggestions()'. Se extrage id-ul utilizatorului. Este creata o lista in
care vor fi adaugate sugestiile. Este parcursa lista de prieteni si pentru
fiecare prieten se verifica daca prietenii acestuia sunt si in lista
utilizatorului dat. Sunt adaugati la sugestii in cazul in care nu sunt gasiti.

- in cazul in care primul cuvant este 'distance', este apelata functia
'get_distance()'. Se obtin id-urile utilizatorilor si se calculeaza distanta
minima dntre acestia cu ajutorul functiei 'bfs_list_graph()'.

- in cazul in care primul cuvant este 'common', este apelata functia
'get_common_friends()'. Se obtin id-urile utilizatorilor si se creaza o lista
in care sunt adaugati prietenii comuni. Se ia fiecare prieten din lista cu
mai putini utilizatori si este cautat in a doua. In cazul in care acesta este
gasit printre prietenii celui de-al doilea utilizator, este adaugat in lista de
prieteni comuni. Daca nu este gasit niciun prieten comun, este afisat un mesaj
de eroare. Altfel, sunt afisati prietenii comuni.

- in cazul in care primul cuvant este 'friends', este apelata functia
'number_of_friends()'. Se extrage id-ul utilizatorului si se afiseaza numarul
de prieteni ai acestuia.

- in cazul in care primul cuvant este 'popular', este apelata functia
'most_popular()'. Se extrage id-ul utilizatorului si este cautat cel mai
popular prieten al acestuia. De fiecare data cand un prieten are un
numar de prieteni mai mare decat max_friends, este actualizat indexul.

### _Task-ul 2 - Postari si Reposturi_

Functia 'handle_input_posts()' separa comenzile si apeleaza functiile
corespunzatoare.

- in cazul in care primul cuvant este 'create', este apelata functia
'add_post()'. Este alocata si initializata o structura de tip post_t. Se
realoca vectorul de postari si este adaugata in vector.

- in cazul in care primul cuvant este 'repost', este apelata functia
'add_repost()'. Este alocata si initializata o structura de tip post_t, 
urmand sa fie adaugata in arbore.

- in cazul in care primul cuvant este 'common-repost', este apelata functia
'print_common_repost()'. Este initializata postarea comuna cu NULL si se
parcurg recursiv toti copiii postarii curente. Daca a mai fost gasita o
postare comuna, se returneaza postarea curenta. Altfel, se retine postarea
gasita. Functia 'print_common_repost()' printeaza mesajul pentru primul
repost comun al celor doua repostari date ca parametri.

- in cazul in care primul cuvant este 'like', este apelata functia
'print_likes()'. Daca repost_id este diferit de 0, se adauga like-ul la
repostarea indicata si este apelata functia 'like_repost()'. Este cautata
recursiv repostarea. Dupa ce este gasita, se verifica daca se va da like sau
dislike. Daca repost_id este 0, se adauga like-ul la postarea data si
este apelata functia 'like_post()'. Este cautat numele utilizatorului care
apreciaza postarea. Daca nu este gasit, este adaugat id-ul in vectorul de 
like-uri. Daca este gasit, inseamna ca se da dislike si este eliminat din
vector. La final se afiseaza mesajele corespunzatoare.

- in cazul in care primul cuvant este 'ratio', este apelata functia 'ratio()'.
Se verifica daca repostarile au un numar de aprecieri mai mare decat postaea
originala si sunt actualizate indexul si numarul maxim de like-uri. La final
se afiseaza mesajul corespunzator fiecarui caz.

- in cazul in care primul cuvant este 'delete', este apelata functia
'print_delete()'. Este stearsa postarea/repostarea data ca parametru cu
ajutorul uneia dintre functiile 'delete_post()' sau 'delete_repost()'. La
final este afisat mesajul corespunzator.

- in cazul in care primul cuvant este 'get-likes', este apelata functia
'print_get_likes()'. Cu ajutorul uneia dintre functiile 'get_post_likes()'
sau 'get_repost_likes()', se afiseaza numarul de like-uri ale postarii/repostarii.

- in cazul in care primul cuvant este 'get-reposts', este apelata functia
'get_reposts()'. Aceasta pacurge aborele si afiseaza toate repostarile
postarii/repostarii unui utilizator cu ajutorul functiilor
'print_reposts_post()' sau 'print_reposts_repost()'

### _Task-ul 3 - Social Media_

Functia 'handle_input_posts()' separa comenzile si apeleaza functiile
corespunzatoare.

- in cazul in care primul cuvant este 'feed', este apelata functia 'feed()'.
Daca numarul de postari cerut este mai mare decat numarul total de postari,
afisam toate postarile curente. Este parcurs vectorul de postari de la cea mai
recenta la cea mai veche. Daca postarea este realizata de un prieten al
utilizatorului dat ca parametru sau de utilizatorul insusi, este afisata
postarea.

- in cazul in care primul cuvant este 'view-profile', este apelata functia
'view_posts()'. Sunt afisate pe rand postarile, apoi repostarile unui
utilizator.

- in cazul in care primul cuvant este 'friends-repost', este apelata functia
'friends-repost()'. Sunt parcurse repostarle si se verifica daca utilizatorul
care a repostat este prieten cu utilizatorul dat. Daca este gasit in lista de
prieteni, este afisat numele acestuia.

- in cazul in care primul cuvant este 'common-group', este apelata functia
'clique()'. Se parcurg utilizatorii. Daca utilizatorul curent si cel dat sunt 
prieteni, cream o lista noua pentru a stoca grupul de prieteni al
utilizatorului curent. Adaugam in ordinea crescatoare a id-urilor toti prietenii
utilizatorului curent in lista. Parcurgem fiecare pereche din lista
grupului pentru a verifica daca oricare doi utilizatori sunt prieteni intre 
ei. Daca nu exista o muchie intre elementul curent si urmatorul gruparea 
curenta nu este o clica. Daca gruparea curenta este o clica si dimensiunea ei 
este mai mare decat cea mai mare clica gasita pana acum, O actualizam.
