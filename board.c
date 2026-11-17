
#include <stdlib.h>
#include <stdbool.h>
#include "board.h"
/**
 * \file board.c
 *
 * \brief Source code associated with \ref board.h
 *
 * \author RIMBAULT Matheo TP2.2
 */
struct board_s{
    size plateau[DIMENSION][DIMENSION];
    bool phase_placement;
    int pieces_restantes[NB_PLAYERS + 1][NB_SIZE + 1];
    player gagnant;
    player joueur_en_mouvement;
    int piece_ligne;
    int piece_colonne;
    size piece_taille;
    int mouvements_restants;
    int ligne_depart;
    int colonne_depart;
    bool sur_piece;
    struct{
        int ligne;
        int colonne;
        direction dir_entree;
    } 
    historique[50];
    int nb_etapes;
};
board new_game(){
    board new_board = malloc(sizeof(struct board_s));
    for(int i = 0; i < DIMENSION; i++){
        for(int j = 0; j < DIMENSION; j++){
            new_board->plateau[i][j] = NONE;
        }
    }
    new_board->phase_placement = true;
    for(int taille = ONE; taille <= THREE; taille++){
        new_board->pieces_restantes[SOUTH_P][taille] = NB_INITIAL_PIECES;
        new_board->pieces_restantes[NORTH_P][taille] = NB_INITIAL_PIECES;
    }
    new_board->gagnant = NO_PLAYER;
    new_board->joueur_en_mouvement = NO_PLAYER;
    new_board->piece_ligne = -1;
    new_board->piece_colonne = -1;
    new_board->piece_taille = NONE;
    new_board->mouvements_restants = -1;
    new_board->ligne_depart = -1;
    new_board->colonne_depart = -1;
    new_board->sur_piece = false;
    new_board->nb_etapes = 0;
    return new_board;
}
board copy_game(board original_game){
    if(original_game == NULL) return NULL;
    board copie = malloc(sizeof(struct board_s));
    for(int i = 0; i < DIMENSION; i++){
        for(int j = 0; j < DIMENSION; j++){
            copie->plateau[i][j] = original_game->plateau[i][j];
        }
    }
    copie->phase_placement = original_game->phase_placement;
    for(int p = 0; p <= NB_PLAYERS; p++){
        for(int t = 0; t <= NB_SIZE; t++){
            copie->pieces_restantes[p][t] = original_game->pieces_restantes[p][t];
        }
    }
    copie->gagnant = original_game->gagnant;
    copie->joueur_en_mouvement = original_game->joueur_en_mouvement;
    copie->piece_ligne = original_game->piece_ligne;
    copie->piece_colonne = original_game->piece_colonne;
    copie->piece_taille = original_game->piece_taille;
    copie->mouvements_restants = original_game->mouvements_restants;
    copie->ligne_depart = original_game->ligne_depart;
    copie->colonne_depart = original_game->colonne_depart;
    copie->sur_piece = original_game->sur_piece;
    copie->nb_etapes = original_game->nb_etapes;
    for(int i = 0; i < original_game->nb_etapes; i++){
        copie->historique[i] = original_game->historique[i];
    }
    return copie;
}
void destroy_game(board game){
    free(game);
}
player next_player(player current_player){
    if(current_player == SOUTH_P){
		return NORTH_P;
	}
    if(current_player == NORTH_P){
		return SOUTH_P;
	}
    return NO_PLAYER;
}
size get_piece_size(board game, int line, int column){
    if(game == NULL){
		return NONE;
	}
    if(line < 0 || line >= DIMENSION){
		return NONE;
	}
    if(column < 0 || column >= DIMENSION){
		return NONE;
	}
    if(game->piece_ligne == line && game->piece_colonne == column && 
        game->piece_taille != NONE && !game->sur_piece){
        return NONE;
    }
    return game->plateau[line][column];
}
player get_winner(board game){
    if(game == NULL){
		return NO_PLAYER;
	}
    return game->gagnant;
}
int southmost_occupied_line(board game){
    if(game == NULL){
		return -1;
	}
    for(int line = 0; line < DIMENSION; line++){
        for(int col = 0; col < DIMENSION; col++){
            if(get_piece_size(game, line, col) != NONE){
                return line;
            }
        }
    }
    return -1;
}
int northmost_occupied_line(board game){
    if(game == NULL){
		return -1;
	}
    for(int line = DIMENSION - 1; line >= 0; line--){
        for(int col = 0; col < DIMENSION; col++){
            if(get_piece_size(game, line, col) != NONE){
                return line;
            }
        }
    }
    return -1;
}
player picked_piece_owner(board game){
    if(game == NULL){
		return NO_PLAYER;
	}
    return game->joueur_en_mouvement;
}
size picked_piece_size(board game){
    if(game == NULL){
		return NONE;
	}
    return game->piece_taille;
}
int picked_piece_line(board game){
    if(game == NULL){
		return -1;
	}
    return game->piece_ligne;
}
int picked_piece_column(board game){
    if(game == NULL){
		return -1;
	}
    return game->piece_colonne;
}
int movement_left(board game){
    if(game == NULL){
		return -1;
	}
    if(game->piece_taille == NONE){
		return -1;
	}
    return game->mouvements_restants;
}
int nb_pieces_available(board game, size piece, player p){
    if(game == NULL){
		return -1;
	}
    if(piece < ONE || piece > THREE){
		return -1;
	}
    if(p != SOUTH_P && p != NORTH_P){
		return -1;
	}
    if(!game->phase_placement){
		return 0;
	}
    return game->pieces_restantes[p][piece];
}
return_code place_piece(board game, size piece, player p, int column){
    if(game == NULL){
		return PARAM;
	}
    if(p != SOUTH_P && p != NORTH_P){
		return PARAM;
	}
    if(piece < ONE || piece > THREE){
		return PARAM;
	}
    if(column < 0 || column >= DIMENSION){
		return PARAM;
	}
    int line;
    if(p == SOUTH_P){
		line = 0;
	}
	else
	{
		line = DIMENSION - 1;
	}
    if(game->plateau[line][column] != NONE){
		return EMPTY;
	}
    if(game->pieces_restantes[p][piece] <= 0){
		return FORBIDDEN;
	}
    if(game->phase_placement == false){
		return FORBIDDEN;
	}
    game->plateau[line][column] = piece;
    game->pieces_restantes[p][piece]--;
    bool placement_termine = true;
    for(int taille = ONE; taille <= THREE; taille++){
        if(game->pieces_restantes[SOUTH_P][taille] > 0){
			placement_termine = false;
		}
        if(game->pieces_restantes[NORTH_P][taille] > 0){
            placement_termine = false;
        }
        
    }
    if(placement_termine == true){
        game->phase_placement = false;
    }
    return OK;
}
return_code pick_piece(board game, player current_player, int line, int column){
    if(game->phase_placement){
		return FORBIDDEN;
	}
    if(game->gagnant != NO_PLAYER){
		return FORBIDDEN;
	}
    if(game == NULL){
		return PARAM;
	}
    if(current_player != SOUTH_P && current_player != NORTH_P){
		return PARAM;
	}
    if(line < 0 || line >= DIMENSION){
		return PARAM;
	}
    if(column < 0 || column >= DIMENSION){
		return PARAM;
	}
    if(game->plateau[line][column] == NONE){
		return EMPTY;
	}
    int ligne_autorisee;
    if(current_player == SOUTH_P){
        ligne_autorisee = southmost_occupied_line(game);
    } 
    else 
    {
        ligne_autorisee = northmost_occupied_line(game);
    }
    if(ligne_autorisee == -1){
		return FORBIDDEN;
	}
    if(line != ligne_autorisee){
		return FORBIDDEN;
	}
    size piece = game->plateau[line][column];
    game->plateau[line][column] = NONE;
    game->joueur_en_mouvement = current_player;
    game->piece_ligne = line;
    game->piece_colonne = column;
    game->piece_taille = piece;
    game->mouvements_restants = piece;
    game->ligne_depart = line;
    game->colonne_depart = column;
    game->sur_piece = false;
    game->nb_etapes = 0;
    return OK;
}
bool is_move_possible(board game, direction dir){
    if(game == NULL){
		return false;
	}
    if(game->piece_taille == NONE){
		return false;
	}
    int ligne_actuelle = game->piece_ligne;
    int colonne_actuelle = game->piece_colonne;
    int nouvelle_ligne = ligne_actuelle;
    int nouvelle_colonne = colonne_actuelle;
    if(dir == NORTH){
		nouvelle_ligne++;
	}
    else if(dir == SOUTH){
		nouvelle_ligne--;
	}
	else if(dir == EAST){
		nouvelle_colonne++;
	}
    else if(dir == WEST){
		nouvelle_colonne--;
	}
    else if(dir == GOAL){
		if(game->joueur_en_mouvement == SOUTH_P && ligne_actuelle == DIMENSION - 1){
			return true; 
        }
        if(game->joueur_en_mouvement == NORTH_P && ligne_actuelle == 0){
            return true; 
        }
        return false;
	}
	else
	{
		return false;
	}
    if(nouvelle_ligne < 0 || nouvelle_ligne >= DIMENSION){
		return false;
	}
    if(nouvelle_colonne < 0 || nouvelle_colonne >= DIMENSION){
		return false;
	}
    size piece_cible = game->plateau[nouvelle_ligne][nouvelle_colonne];
    if(game->mouvements_restants == 1){
        return true;  
    }
    if(piece_cible == NONE){
		return true;
	}
	return false;
}
return_code move_piece(board game, direction dir){
    if(game == NULL){
		return EMPTY;
	}
    if(game->piece_taille == NONE){
		return EMPTY;
	}
    if(game->mouvements_restants == 0 && game->sur_piece){
        size piece_dessous = game->plateau[game->piece_ligne][game->piece_colonne];
        if(piece_dessous != NONE){
            game->mouvements_restants = piece_dessous;
            game->sur_piece = false;
        } 
        else 
        {
            return FORBIDDEN;
        }
    }
    if(is_move_possible(game, dir) == false){
		return FORBIDDEN;
	}
    if(dir != GOAL){
		int ligne_actuelle = game->piece_ligne;
		int colonne_actuelle = game->piece_colonne;
		for(int i = 0; i < game->nb_etapes; i++){
			int ligne_depart_i, colonne_depart_i;
			if(i == 0){
				ligne_depart_i = game->ligne_depart;
				colonne_depart_i = game->colonne_depart;
			}
			else
			{
				ligne_depart_i = game->historique[i-1].ligne;
				colonne_depart_i = game->historique[i-1].colonne;
			}
			if(ligne_depart_i == ligne_actuelle && colonne_depart_i == colonne_actuelle && game->historique[i].dir_entree == dir){
				return FORBIDDEN;
			}
		}
	}
    int ligne_actuelle = game->piece_ligne;
    int colonne_actuelle = game->piece_colonne;
    if(dir == GOAL){
        game->gagnant = game->joueur_en_mouvement;
        game->joueur_en_mouvement = NO_PLAYER;
        game->piece_ligne = -1;
        game->piece_colonne = -1;
        game->piece_taille = NONE;
        game->mouvements_restants = -1;
        game->sur_piece = false;
        return OK;
    }
    int nouvelle_ligne = ligne_actuelle;
    int nouvelle_colonne = colonne_actuelle;
	if(dir == NORTH){
		nouvelle_ligne++;
	}
	else if(dir == SOUTH){
		nouvelle_ligne--;
	}
	else if(dir == EAST){
		nouvelle_colonne++;
	}
	else if(dir == WEST){
		nouvelle_colonne--;
	}
	else
	{
		return PARAM;
	}
    if(nouvelle_ligne < 0 || nouvelle_ligne >= DIMENSION){
		return PARAM;
	}
    if(nouvelle_colonne < 0 || nouvelle_colonne >= DIMENSION){
		return PARAM;
	}
    if(game->nb_etapes < 50){
        game->historique[game->nb_etapes].ligne = nouvelle_ligne;
        game->historique[game->nb_etapes].colonne = nouvelle_colonne;
        game->historique[game->nb_etapes].dir_entree = dir;
        game->nb_etapes++;
    }
    game->piece_ligne = nouvelle_ligne;
    game->piece_colonne = nouvelle_colonne;
    game->mouvements_restants--;
    size piece_cible = game->plateau[nouvelle_ligne][nouvelle_colonne];
    if(piece_cible != NONE && game->mouvements_restants == 0){
        game->sur_piece = true;
    } 
    else if(game->mouvements_restants == 0){
        game->plateau[nouvelle_ligne][nouvelle_colonne] = game->piece_taille;
        game->joueur_en_mouvement = NO_PLAYER;
        game->piece_ligne = -1;
        game->piece_colonne = -1;
        game->piece_taille = NONE;
        game->mouvements_restants = -1;
        game->sur_piece = false;
        game->nb_etapes = 0;
    }
    return OK;
}
return_code swap_piece(board game, int target_line, int target_column){ 
    if(game == NULL){
		return EMPTY;
	}
    if(game->piece_taille == NONE){
		return EMPTY;
	}
    if(game->sur_piece == false){
		return EMPTY;
	}
    if(game->mouvements_restants != 0){
		return EMPTY;
	}
    if(target_line < 0 || target_line >= DIMENSION){
		return PARAM;
	}
    if(target_column < 0 || target_column >= DIMENSION){
		return PARAM;
	}
    if(game->plateau[target_line][target_column] != NONE){
		return FORBIDDEN;
	}
    size piece_dessous = game->plateau[game->piece_ligne][game->piece_colonne];
    if(piece_dessous == NONE){
		return EMPTY;
	}
    game->plateau[game->piece_ligne][game->piece_colonne] = game->piece_taille;
    game->plateau[target_line][target_column] = piece_dessous;
    game->joueur_en_mouvement = NO_PLAYER;
    game->piece_ligne = -1;
    game->piece_colonne = -1;
    game->piece_taille = NONE;
    game->mouvements_restants = -1;
    game->sur_piece = false;
    game->nb_etapes = 0;
    return OK;
}
return_code cancel_movement(board game){
    if(game == NULL){
		return EMPTY;
	}
    if(game->piece_taille == NONE){
		return EMPTY;
	}
    game->plateau[game->ligne_depart][game->colonne_depart] = game->piece_taille;
    game->joueur_en_mouvement = NO_PLAYER;
    game->piece_ligne = -1;
    game->piece_colonne = -1;
    game->piece_taille = NONE;
    game->mouvements_restants = -1;
    game->ligne_depart = -1;
    game->colonne_depart = -1;
    game->sur_piece = false;
    game->nb_etapes = 0;
    return OK;
}
return_code cancel_step(board game){
    if(game == NULL){
		return EMPTY;
	}
    if(game->piece_taille == NONE){
		return EMPTY;
	}
    if(game->nb_etapes == 0){
        return cancel_movement(game);
    }
    game->nb_etapes--;
    if(game->nb_etapes == 0){
        game->piece_ligne = game->ligne_depart;
        game->piece_colonne = game->colonne_depart;
    } 
    else 
    {
        game->piece_ligne = game->historique[game->nb_etapes - 1].ligne;
        game->piece_colonne = game->historique[game->nb_etapes - 1].colonne;
    }
    game->mouvements_restants++;
    game->sur_piece = false;
    return OK;
}
