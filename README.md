
# Progetto Programmazione e laboratorio A.A. 2022/2023

Lo scopo del progetto è scrivere un parser (tramite grammatiche context-free) di file in formato json. Il contenuto del file deve essere salvato in un container C++ che permetta l’accesso (tramite iteratori e overloading di operatori) del contenuto letto dal file json.


### 1. Formato json

Si vuole scrivere un parser per una variante _semplificata_ di json – un formato testuale per lo scambio di informazione sul Web. **Importante**: usate la descrizione json fornita di seguito e non quella trovata in rete.

Un file json contiene dati in una struttura ad albero (ricorsiva), simile al formato XML. A differenza di XML, il formato json ammette sia **liste** di valori che **dizionari** (definiti nel dettaglio qui sotto). Globalmente, un file json è una lista, un dizionario, un numero (double), un booleano, una stringa, oppure null.

  

### 2. Obiettivo finale ed esempi di come verrà testato il codice

Il container json creato può essere usato come un editor di file in formato json. Una volta caricato, il file può essere manipolato/navigato usando gli operatori definiti. Per esempio, supponiamo che `std::cin` contenga lo stream:

    [
        1,
        {
            "prima chiave" : 5,
            "seconda chiave" : [4.12,2,true],
            "terza chiave" : "una stringa",
            "quarta chiave" : {"a" : 4, "b" : [4,5]}
        },
        3
    ]

allora, il seguente codice è valido e deve stampare a schermo “4”: 

    json j;
    std::cin >> j;
    json& y = *(++j.begin_list());
    std::cout << y["quarta chiave"]["a"];


Inoltre, la classe ci permette di modificare un oggetto `json`. Per esempio, se la variabile 

    json z;

contiene i dati del file json

    {"c" : 5, "d" : 6}

e `j` è la variabile di tipo `json` definita sopra, allora dopo l’assegnamento

    (*(++j.begin_list()))["prima chiave"] = z

 
La variabile `j` deve contenere i seguenti dati: 

    [
        1,
        {
            "prima chiave" : {"c" : 5, "d" : 6},
            "seconda chiave" : [4.12,2,true],
            "terza chiave" : "una stringa",
            "quarta chiave" : {"a" : 4, "b" : [4,5]}
        },
        3
    ]


 

