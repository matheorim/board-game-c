# Jeu de plateau en C

🇬🇧 [English version](README.md)

Jeu de plateau stratégique pour deux joueurs, joué dans le terminal, écrit en C pendant ma première année de BUT Informatique à l'IUT Grand Ouest Normandie (Caen).

## Le jeu

Deux joueurs, **Nord** et **Sud**, s'affrontent sur un plateau de 6×6 cases avec un but de chaque côté. Le premier qui amène une pièce dans le but adverse gagne.

- 12 pièces partagées : quatre de chacune des trois tailles (1, 2 et 3). Les pièces n'appartiennent à aucun joueur.
- **Phase de placement :** chaque joueur place six pièces sur la rangée la plus proche de lui.
- **Phase de déplacement :** à son tour, un joueur prend une pièce de sa rangée et la déplace d'un nombre de pas égal à sa taille, dans les quatre directions. Seul le dernier pas peut finir sur une case occupée, auquel cas la pièce rebondit.

Les règles complètes sont documentées dans l'en-tête `board.h`.

## Ce que j'ai écrit

| Fichier | Contenu | Auteur |
|---|---|---|
| `board.c` | Implémentation du moteur de jeu : état du plateau, placement, prise et déplacement des pièces, rebonds, détection de la victoire | Mathéo Rimbault |
| `sae1-2.c` | Jeu en terminal : affichage du plateau, saisies, boucle de jeu, fin de partie | Mathéo Rimbault |
| `board.h` | Interface et documentation du moteur de jeu, **fournie par l'équipe pédagogique** (Paul Dorbec) | P. Dorbec |

## Compilation et lancement

Prérequis : `gcc` et `make` (Linux, macOS, ou WSL sous Windows).

```bash
make
./jeu
```

Nettoie les fichiers de compilation avec `make clean`.

## Comment jouer

À chaque tour, le programme affiche le plateau et demande tes actions (taille de la pièce, colonne, direction). Suis les instructions dans le terminal.

## Notions utilisées

- Structures et types opaques (le type `board` est défini dans `board.c`)
- Allocation et libération de mémoire (`new_game` / `destroy_game`)
- Programmation modulaire avec fichiers d'en-tête
- Validation des saisies et boucle de jeu
- Compilation avec `make`
