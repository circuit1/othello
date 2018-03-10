How and what each group member contributed:

Sunny Cui: wrote alpha-beta pruning and iterative deepening,
			wrote and debugged
			minimax, wrote original heuristic and
			corrected it

Tina Wang: debugged minimax and alpha-beta pruning,
			got the AI working with random moves
			originally, testminimax, rewrote
			heuristic to beat SimplePlayer

Our strategy:
	Our heuristic weighs number of legal moves
	really heavily in the first moves of the game
	since mobility is very important in the beginning
	part of the game. We also used alpha-beta pruning
	and iterative deepening to let us pick
	the best move and repeatedly run
	minimax without running out of time, respectively.
	