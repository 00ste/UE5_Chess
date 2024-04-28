# UE5_Chess

Il classico gioco degli Scacchi, realizzato in Unreal Engine 5.2, come progetto del corso di Progettazione e Analisi di Algoritmi, A.A. 2023/2024, UNIGE, sviluppato da Amalberti Stefano.

## Features

- Regole di base degli Scacchi (no en-passant e arrocco)
- Giocatore avversario con mosse casuali e con algoritmo Minimax (selezione del giocatore all'interno del metodo `ACH_GameMode::BeginPlay`)
- Storico delle mosse in notazione algebrica standard (SAN) con replay
- Gestione dei pareggi automatici con la regola delle 75 mosse.
