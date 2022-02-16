# Modbus HR address updater

Il programma cerca la stringa `R_addr=$` e ne modifica il valore successivo.

* input file: `in.txt`
* output file: `out.txt`

Il programma richiede all'utente l'indirizzo esadecimale da cui partire.

Le linee che iniziano con `(*` sono considerate commenti e non vengono modificate.

---

Se l'eseguibile viene lanciato da riga di comando, è possibile aggiungere l'opzione
`-v` per far stampare gli indirizzi modbus con il minor numero di caratteri possibili,
invece del default 4.

Esempio: `R_addr=$601` al posto di `R_addr=$0601`
