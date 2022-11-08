sound_files_welcome BYTE "../res/welcome.wav", 0
sound_files_game BYTE "../res/game.wav", 0
sound_files_clicked BYTE "../res/clicked.wav", 0
sound_files_match BYTE "../res/match.wav", 0
sound_files DWORD offset sound_files_welcome, offset sound_files_game, offset sound_files_clicked, offset sound_files_match
