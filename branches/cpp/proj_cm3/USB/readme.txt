* Information sur les projets du repertoire USB

Le repertoire "USB" contient des programme de demo pour differentes classes USB. Une classe USB correspond à un type d'utilisation 
de l'USB et chaque classe defini un certain nombre d'interface et de message que le peripherique USB doit respecter.

Des classes prevue par le consortium USB, il y en pas mal. Lorsque l'on branche son peripherique USB, le periph indique son identifiant
de vendeur (VID), de produit (PID) et la classe USB implementée. Beaucoup de constructeur choisissent d'implementer des classes non 
conforme à l'USB Forum (des classes proprietaires, donc), et doivent donc fournir un driver, que Windows associe au peripherique
grace au VID et PID.

Par contre, si on tape dans les classes generiques du consortium USB Forum, pour beaucoup, Windows fourni deja le driver. C'est tout l'interet,
pas besoin de se gratter un driver pour Windows. C'est le cas du MSC (Masse media storage class) des clefs USB, des peripheriques HID et dans une
moindre mesure, des peripheriques de communication (les modems) qui implementent la classe CDC.

En effet, pour la classe CDC, Windows fourni bien le driver (usbser.sys), mais chez Microsoft, ce sont des joueurs: Windows ne sait pas associer tout seul
ce driver à un peripherique CDC, du moins, pas toujours: dasn le cadre des demo, les identifiants VID et PID de ST permettent à WIndows d'aller
chercher son driver tout seul. 

Les demo presentes dans ce repertoire couvrent les classes suivante:

     CDC: Communication Device Class
     C'est probablement le type de classe que l'on va utiliser le plus: en gros, ca crée un port COM virtuel, une UART sur USB en somme.

     HID: Human Interface Device
     C'est une classe plus dedié que la CDC: en gros, vu du PC, le peripherique possede des registres en entrée et d'autres en sortie, dans
     lesquels le PC peut lire et/ou ecrire. Si ce "registre" est connecté a un port, les lignes du micro peuvent "baguotter". Si c'est
     une fonction (un rapport de PWM), on peut le faire varier. Pour info, c'est cette classe qu'implemente la sonde JTAG ULink-ME de Keil.

     DFU: Device Firmware Upgrade
     C'est une classe particuliere qui permet de mettre a jour le soft (le firmware) du micro. Inconnu jusqu'a l'arrivé de l'Iphone et l'Ipod,
     c'est en passe de devenir une classe generique  