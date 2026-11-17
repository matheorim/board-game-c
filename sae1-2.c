
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "board.h"

void affiche_board(board game) {
    printf("\n   PLATEAU DE JEU\n\n");
    printf("  ");
    for (int col = 0; col < DIMENSION; col++) {
        printf(" %d ", col);
        if (col < DIMENSION - 1) {
            printf(" ");
        }
    }
    printf("\n       BUT NORD \n");
    int piece_line = picked_piece_line(game);
    int piece_col = picked_piece_column(game);
    size piece_size = picked_piece_size(game);
    for (int line = DIMENSION - 1; line >= 0; line--) {
        printf("%d ", line);
        for (int col = 0; col < DIMENSION; col++) {
            if (piece_line == line && piece_col == col && piece_size != NONE) {
                printf("[%d]", piece_size);
            } else {
                size taille = get_piece_size(game, line, col);
                if (taille == NONE) {
                    printf(" . ");
                } else {
                    printf(" %d ", taille);
                }
            }
            if (col < DIMENSION - 1) {
                printf("-");
            }
        }
        printf("\n");
        if (line > 0) {
            printf("   ");
            for (int col = 0; col < DIMENSION; col++) {
                printf("|");
                if (col < DIMENSION - 1) {
                    printf("   ");
                }
            }
            printf("\n");
        }
    }
    printf("       BUT SUD \n\n");
}
void affiche_pieces_disponibles(board game, player joueur) {
    printf("Pieces disponibles a placer:\n");
    printf("  Taille 1: %d restante(s)\n", nb_pieces_available(game, ONE, joueur));
    printf("  Taille 2: %d restante(s)\n", nb_pieces_available(game, TWO, joueur));
    printf("  Taille 3: %d restante(s)\n", nb_pieces_available(game, THREE, joueur));
}
void affiche_mouvements_possibles(board game) {
    printf("\nMouvements possibles: ");
    bool aucun = true;
    if (is_move_possible(game, NORTH)) {
        printf("N(ord) ");
        aucun = false;
    }
    if (is_move_possible(game, SOUTH)) {
        printf("S(ud) ");
        aucun = false;
    }
    if (is_move_possible(game, EAST)) {
        printf("E(st) ");
        aucun = false;
    }
    if (is_move_possible(game, WEST)) {
        printf("W(est) ");
        aucun = false;
    }
    if (is_move_possible(game, GOAL)) {
        printf("G(OAL!) ");
        aucun = false;
    }
    if (aucun) {
        printf("Aucun mouvement possible!");
    }
    printf("\n");
}
void initialisation(board game) {
    int compteur_pieces = 0;
    player joueur_actuel = SOUTH_P;
    printf("\n   PHASE DE PLACEMENT DES PIECES\n\n");
    printf("Chaque joueur doit placer 6 pieces:\n");
    printf("  - 2 pieces de taille 1\n");
    printf("  - 2 pieces de taille 2\n");
    printf("  - 2 pieces de taille 3\n\n");
    while (compteur_pieces < 12) {
        affiche_board(game);
        if (joueur_actuel == SOUTH_P) {
            printf(" Tour du joueur SUD \n");
        } else {
            printf(" Tour du joueur NORD \n");
        }
        affiche_pieces_disponibles(game, joueur_actuel);
        int taille_saisie, colonne_saisie;
        size taille;
        printf("\nChoisissez la taille de la piece (1/2/3): ");
        scanf("%d", &taille_saisie);
        if (taille_saisie == 1) {
            taille = ONE;
        } else if (taille_saisie == 2) {
            taille = TWO;
        } else if (taille_saisie == 3) {
            taille = THREE;
        } else {
            printf("ERREUR: Taille invalide. Choisissez 1, 2 ou 3.\n");
            continue;
        }
        printf("Choisissez la colonne (0-%d): ", DIMENSION - 1);
        scanf("%d", &colonne_saisie);
        return_code res = place_piece(game, taille, joueur_actuel, colonne_saisie);
        if (res == OK) {
            compteur_pieces++;
            printf(" Piece placee avec succes!\n");
            if (joueur_actuel == SOUTH_P) {
                joueur_actuel = NORTH_P;
            } else {
                joueur_actuel = SOUTH_P;
            }
        } else {
            if (res == PARAM) {
                printf("ERREUR: Parametres invalides (colonne hors du plateau).\n");
            } else if (res == EMPTY) {
                printf("ERREUR: Cette case est deja occupee.\n");
            } else if (res == FORBIDDEN) {
                printf("ERREUR: Vous avez deja place toutes vos pieces de cette taille.\n");
            }
        }
    }
    printf("\n  Toutes les pieces sont placees!\n");
    printf("       LA PARTIE COMMENCE!\n\n");
}
direction char_to_direction(char c) {
    if (c == 'N' || c == 'n') {
        return NORTH;
    }
    if (c == 'S' || c == 's') {
        return SOUTH;
    }
    if (c == 'E' || c == 'e') {
        return EAST;
    }
    if (c == 'W' || c == 'w') {
        return WEST;
    }
    if (c == 'G' || c == 'g') {
        return GOAL;
    }
    return NORTH;
}
bool tour_de_jeu(board game, player p) {
    int c_depart, l_depart;
    return_code resultat;
    if (p == SOUTH_P) {
        printf("Joueur SUD, selection de la piece\n");
        l_depart = southmost_occupied_line(game);
    } else {
        printf("Joueur NORD, selection de la piece\n");
        l_depart = northmost_occupied_line(game);
    }
    if (l_depart == -1) {
        printf("ERREUR: Aucune piece disponible.\n");
        return false;
    }
    printf("Vous devez choisir une piece sur la ligne %d\n", l_depart);
    printf("Choisissez la colonne (0-%d): ", DIMENSION - 1);
    scanf("%d", &c_depart);
    resultat = pick_piece(game, p, l_depart, c_depart);
    if (resultat != OK) {
        if (resultat == PARAM) {
            printf("ERREUR: Coordonnees invalides.\n");
        } else if (resultat == EMPTY) {
            printf("ERREUR: Aucune piece a cette position.\n");
        } else if (resultat == FORBIDDEN) {
            printf("ERREUR: Vous ne pouvez pas selectionner cette piece.\n");
        }
        return false;
    }
    size taille_piece = picked_piece_size(game);
    printf(" Piece de taille %d selectionnee!\n", taille_piece);
    printf("Vous devez effectuer %d deplacement(s).\n", taille_piece);
    while (true) {
        if (picked_piece_size(game) == NONE) {
            return true;
        }
        if (movement_left(game) == 0) {
            int ligne_actuelle = picked_piece_line(game);
            int col_actuelle = picked_piece_column(game);
            size piece_dessous = get_piece_size(game, ligne_actuelle, col_actuelle);
            if (piece_dessous != NONE) {
                affiche_board(game);
                printf("SITUATION SPECIALE\n");
                printf("Votre piece est sur une autre piece de taille %d!\n", piece_dessous);
                printf("Vous avez deux options:\n");
                printf("  R - Rebondir (continuer avec %d deplacements)\n", piece_dessous);
                printf("  E - Echanger (placer la piece dessous ailleurs)\n");
                printf("  A - Annuler le mouvement\n");
                printf("Votre choix: ");
                char action;
                scanf(" %c", &action);
				if (action == 'R' || action == 'r') {
					printf("Vous rebondissez! %d nouveaux deplacements disponibles.\n", piece_dessous);
					printf("Choisissez une direction pour continuer (N/S/E/W/G): ");
					char dir_char;
					scanf(" %c", &dir_char);
					direction d = char_to_direction(dir_char);
					if (!is_move_possible(game, d)) {
						printf("ERREUR: Mouvement impossible dans cette direction.\n");
						continue;
					}
					resultat = move_piece(game, d);
					if (resultat != OK) {
						printf("ERREUR: Le rebond a échoué.\n");
						cancel_movement(game);
						return false;
					}
					printf("Rebond effectué!\n");
					continue; 
				} else if (action == 'E' || action == 'e') {
                    int nouvelle_ligne, nouvelle_colonne;
                    printf("Nouvelle position pour la piece ejectee:\n");
                    printf("  Ligne (0-%d): ", DIMENSION - 1);
                    scanf("%d", &nouvelle_ligne);
                    printf("  Colonne (0-%d): ", DIMENSION - 1);
                    scanf("%d", &nouvelle_colonne);
                    resultat = swap_piece(game, nouvelle_ligne, nouvelle_colonne);
                    if (resultat == OK) {
                        printf("Echange effectue avec succes!\n");
                        return true;
                    } else {
                        if (resultat == EMPTY) {
                            printf("ERREUR: Echange impossible.\n");
                        } else if (resultat == PARAM) {
                            printf("ERREUR: Position hors du plateau.\n");
                        } else if (resultat == FORBIDDEN) {
                            printf("ERREUR: Position non libre.\n");
                        }
                        cancel_movement(game);
                        return false;
                    }
                } else {
                    printf("Annulation du mouvement.\n");
                    cancel_movement(game);
                    return false;
                }
            } else {
                return true;
            }
        }
        if (movement_left(game) <= 0) {
            return true;
        }
        affiche_board(game);
        printf("Deplacements restants: %d\n", movement_left(game));
        affiche_mouvements_possibles(game);
        printf("\nVotre choix (N/S/E/W/G pour Goal, A pour annuler): ");
        char dir_char;
        scanf(" %c", &dir_char);
        if (dir_char == 'A' || dir_char == 'a') {
            cancel_movement(game);
            printf("Mouvement annule. Choisissez une autre piece.\n");
            return false;
        }
        direction d = char_to_direction(dir_char);
        if (!is_move_possible(game, d)) {
            printf("ERREUR: Mouvement impossible dans cette direction.\n");
            continue;
        }
        resultat = move_piece(game, d);
        if (resultat != OK) {
            if (resultat == EMPTY) {
                printf("ERREUR: Aucune piece selectionnee.\n");
            } else if (resultat == PARAM) {
                printf("ERREUR: Mouvement hors du plateau.\n");
            } else if (resultat == FORBIDDEN) {
                printf("ERREUR: Mouvement interdit.\n");
            }
            return false;
        }
        printf(" Deplacement effectue!\n");
    }
    return true;
}
int main() {
    printf("\n    BIENVENUE DANS LE JEU DE PLATEAU\n\n");
    printf("Objectif: Amenez une piece dans le but adverse!\n\n");
    board game = new_game();
    initialisation(game);
    player joueur = SOUTH_P;
    int tour_numero = 1;
    while (get_winner(game) == NO_PLAYER) {
        printf("\n           TOUR #%d\n", tour_numero);
        affiche_board(game);
        if (joueur == SOUTH_P) {
            printf(" Au tour du joueur SUD \n");
        } else {
            printf(" Au tour du joueur NORD \n");
        }
        if (!tour_de_jeu(game, joueur)) {
            printf("\nTour incomplet. Reessayez.\n");
        } else {
            if (joueur == SOUTH_P) {
                joueur = NORTH_P;
            } else {
                joueur = SOUTH_P;
            }
            tour_numero++;
        }
    }
    affiche_board(game);
    printf("\n           FIN DE PARTIE!\n\n");
    player gagnant = get_winner(game);
    if (gagnant == SOUTH_P) {
        printf("     VICTOIRE DU JOUEUR SUD!\n\n");
    } else {
        printf("     VICTOIRE DU JOUEUR NORD!\n\n");
    }
    printf("Felicitations!\n");
    destroy_game(game);
    return 0;
}
