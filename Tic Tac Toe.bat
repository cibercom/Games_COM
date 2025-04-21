@echo off
 setlocal enabledelayedexpansion
 title .:TIC TAC TOE:.
 mode con: cols=40 lines=20
 color 02
 cls



 goto :main

 :check
     if !_win1!==XXX (
         echo END OF GAME
         pause
     )
     if !_win2!==XXX (
         echo END OF GAME
         pause
     )
     if !_win3!==XXX (
         echo END OF GAME
         pause
     )
     if !_win4!==XXX (
         echo END OF GAME
         pause
     )
     if !_win5!==XXX (
         echo END OF GAME
         pause
     )
     if !_win6!==XXX (
         echo END OF GAME
         pause
     )
     if !_win7!==XXX (
         echo END OF GAME
         pause
     )
     if !_win8!==XXX (
         echo END OF GAME
         pause
     )
 goto :eof

 :player2turn
     choice /c 123456789 /n /m "!_player2!, it's your turn!"
     if !errorlevel!==1 (
             if !_f1!==1 (
                 set _f1=!_player2symbol!
                 goto :eof
             ) else (
                 goto :player1turn
                 )
         )
         if !errorlevel!==2 (
             if !_f2!==2 (
                 set _f2=!_player2symbol!
                 goto :eof
             ) else (
                 goto :player1turn
             )
         )
         if !errorlevel!==3 (
             if !_f3!==3 (
                 set _f3=!_player2symbol!
                 goto :eof
             ) else (
                 goto :player1turn
             )
         )
         if !errorlevel!==4 (
             if !_f4!==4 (
                 set _f4=!_player2symbol!
                 goto :eof
             ) else (
                 goto :player1turn
             )
         )
         if !errorlevel!==5 (
             if !_f5!==5 (
                 set _f5=!_player2symbol!
                 goto :eof
             ) else (
                 goto :player1turn
             )
         )
         if !errorlevel!==6 (
             if !_f6!==6 (
                 set _f6=!_player2symbol!
                 goto :eof
             ) else (
                 goto :player1turn
             )
         )
         if !errorlevel!==7 (
             if !_f7!==7 (
                 set _f7=!_player2symbol!
                 goto :eof
             ) else (
                 goto :player1turn
             )
         )
         if !errorlevel!==8 (
             if !_f8!==8 (
                 set _f8=!_player2symbol!
                 goto :eof
             ) else (
                 goto :player1turn
             )
         )
         if !errorlevel!==9 (
             if !_f9!==9 (
                 set _f9=!_player2symbol!
                 goto :eof
             ) else (
                 goto :player1turn
             )
         )
 goto :eof

 :player1turn
     choice /c 123456789 /n /m "!_player1!, it's your turn!"
         if !errorlevel!==1 (
             if !_f1!==1 (
                 set _f1=!_player1symbol!
                 goto :eof
             ) else (
                 goto :player1turn
                 )
         )
         if !errorlevel!==2 (
             if !_f2!==2 (
                 set _f2=!_player1symbol!
                 goto :eof
             ) else (
                 goto :player1turn
             )
         )
         if !errorlevel!==3 (
             if !_f3!==3 (
                 set _f3=!_player1symbol!
                 goto :eof
             ) else (
                 goto :player1turn
             )
         )
         if !errorlevel!==4 (
             if !_f4!==4 (
                 set _f4=!_player1symbol!
                 goto :eof
             ) else (
                 goto :player1turn
             )
         )
         if !errorlevel!==5 (
             if !_f5!==5 (
                 set _f5=!_player1symbol!
                 goto :eof
             ) else (
                 goto :player1turn
             )
         )
         if !errorlevel!==6 (
             if !_f6!==6 (
                 set _f6=!_player1symbol!
                 goto :eof
             ) else (
                 goto :player1turn
             )
         )
         if !errorlevel!==7 (
             if !_f7!==7 (
                 set _f7=!_player1symbol!
                 goto :eof
             ) else (
                 goto :player1turn
             )
         )
         if !errorlevel!==8 (
             if !_f8!==8 (
                 set _f8=!_player1symbol!
                 goto :eof
             ) else (
                 goto :player1turn
             )
         )
         if !errorlevel!==9 (
             if !_f9!==9 (
                 set _f9=!_player1symbol!
                 echo !_win1!
                 echo !_player1symbol!
                 pause
                 goto :eof
             ) else (
                 goto :player1turn
             )
         )
 goto :eof

 ::displaying the map
 :map
     echo.
     echo.
     echo.
     echo           !_f7! ^| !_f8! ^| !_f9!
     echo           ---------
     echo           !_f4! ^| !_f5! ^| !_f6!
     echo           ---------
     echo           !_f1! ^| !_f2! ^| !_f3!
     echo.
     echo.
     echo.
 goto :eof


 ::setting variables
 :setvar
     set _f1=1
     set _f2=2
     set _f3=3
     set _f4=4
     set _f5=5
     set _f6=6
     set _f7=7
     set _f8=8
     set _f9=9

     set _win1=!_f7!!_f8!!_f9!
     set /a _win2=!_f4!+!_f5!+!_f6!
     set /a _win3=!_f1!+!_f2!+!_f3!
     set /a _win4=!_f7!+!_f4!+!_f1!
     set /a _win5=!_f8!+!_f5!+!_f2!
     set /a _win6=!_f9!+!_f6!+!_f3!
     set /a _win7=!_f7!+!_f5!+!_f3!
     set /a _win8=!_f9!+!_f5!+!_f1!
 goto :eof

 ::player settings
 :setup
     echo.
     set /p _player1=Enter name for Player 1: 
     echo.
     choice /c xo /n /m "!_player1! do you want to play as X or O?"
         if !errorlevel!==1 (
             set _player1symbol=X
             set _player2symbol=O
         ) else (
             set _player1symbol=O
             set _player2symbol=X
         )
     echo.
     echo.
     set /p _player2=Enter name for Player 2: 
     echo.
     cls
     echo.
     echo.
     echo.
     echo !_player1!(!_player1symbol!) vs. !_player2!(!_player2symbol!)
     pause>nul
 goto :eof

 :main
 setlocal

     call :setup
     cls
     call :setvar

     call :map
     call :player1turn
     cls
     call :map
     call :player2turn
     cls
     call :map
     call :player1turn
     cls
     call :map
     call :player2turn
     cls
     call :map
     call :player1turn
     call :check


 endlocal
 goto :eof