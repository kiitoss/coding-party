# Coding Party

La Coding Party est une épreuve de licence 3 informatique à l'université de Saint-Etienne, proposée par le professeur M. Ezequel.

L'objectif de cette épreuve est de développer un projet de programmation système dans un temps restreint de 48H. Une fois ce temps écoulé, une archive du projet doit être envoyée et une soutenance se déroule dans les jours qui suivent.

## Présentation du projet
On se propose de simuler l'activité d'un garage. Dans ce garage, deux catégories d'employés, des chefs d'ateliers et des mécaniciens, s'activent pour satisfaire les requêtes de clients. Plus précisément :

- chaque client contacte le chef d'atelier le moins occupé (comme dans la vraie vie), et attend une réponse de sa part.
- les chefs d'ateliers sont chargés de faire le lien entre les demandes des clients et les mécaniciens: à la réception d'un client, un chef d'atelier poste une demande de travail aux mécaniciens; lorsqu'il reçoit une réponse des mécaniciens, il la répercute au client concerné.
- les mécaniciens sont chargés du travail. Ils récupèrent les demandes de travaux des chefs d'atelier, exécutent les travaux, et préviennent les chefs d'ateliers lorsque le travail est terminé.
L'application sera composée d'un processus initial qui va se charger de créer et d'initialiser les IPC nécessaires à l'application, et d'un certain nombre de processus fils.
