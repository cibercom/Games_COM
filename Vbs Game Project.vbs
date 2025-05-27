Public Options
Public X
Public Y
Public Var
Public Var2
Public Var3

Var = ""
Var2 = ""
Var3 = ""
Options = vbAbort
X = 10
Y = ""

do While true
	Call add()
Loop

Function Zr(z)
	Var = ""
	For i = 1 To z Step 1
		Var = Var+"`"
	Next
End Function
Function Zr2(z)
	Var2 = ""
	For i = 1 To z Step 1
		Var2 = Var2+"``"
	Next
End Function
Function Cover()
	Var3 = ""
	For i = 1 To 60 Step 1
		Var3 = Var3+"``"
	Next
End Function

Function add()
	Y = ""
	For i = 1 To X Step 1
		Y = Y+" "
	Next
	Call Zr(X*2-1)
	Call Zr2(59-X)
	Call Cover()
	retval = MsgBox(Var3+""&Var3+""&Var+"****"+Var2+""&Var+"****"+Var2+""&Var+"``**``"+Var2+""&Var3+""&Var3+"```````````", Options, "Vbs Game")
	If retVal = vbYes then
		If X-5 > -1 then
			X=X-5
		End If
	End If
	If retVal = vbNo then

		If X+5 < 60 then
			X=X+5
		End If
	End If
	If retVal = vbCancel then
		X=0
		WScript.Quit()
	End If
End Function
