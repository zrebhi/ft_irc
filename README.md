#ft_irc

- map<name, client> => probleme si le nick change en cours ?
    -> attribuer un numero unique de client a la connexion au serveur (voir prendre le numero de socket qui est unique)
- channelName dans join => substr(1) si il ne commence pas par # ?
    -> check le # et sortir si c'est dead
