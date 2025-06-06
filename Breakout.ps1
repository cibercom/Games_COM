﻿#Generated Form Function
function GenerateForm {
########################################################################
# Code Generated By: SAPIEN Technologies PrimalForms (Community Edition) v1.0.10.0
# Generated On: 2015-05-26 16:02
# Generated By: rikard
########################################################################

#region Import the Assemblies
[reflection.assembly]::loadwithpartialname("System.Windows.Forms") | Out-Null
[reflection.assembly]::loadwithpartialname("System.Drawing") | Out-Null
#endregion

#region Generated Form Objects
$form1 = New-Object System.Windows.Forms.Form
$Textboxnob = New-Object System.Windows.Forms.TextBox
$label2 = New-Object System.Windows.Forms.Label
$label1 = New-Object System.Windows.Forms.Label
$textBoxScore = New-Object System.Windows.Forms.TextBox
$buttonStop = New-Object System.Windows.Forms.Button
$buttonStart = New-Object System.Windows.Forms.Button
$InitialFormWindowState = New-Object System.Windows.Forms.FormWindowState
#endregion Generated Form Objects

#----------------------------------------------
#Generated Event Script Blocks
#----------------------------------------------
#Provide Custom Code for events specified in PrimalForms.
$buttonStop_OnClick= 
{
#TODO: Place custom script here
	$form1.close()
}
	
#class ball:
	
$buttonStart_OnClick=
	{
		$score=0
		$nob=3 # number of balls
		$ball= new-object Drawing.Rectangle 100, 180, 20, 20
		$ballSpeedX=10
		$BallSpeedY=10
		$bricky = 80
		
		[System.Collections.ArrayList]$bricks = @()
		for ($y = 1; $y -lt 5; $y++)
		{
			$brickx = 40
			for ($i = 1; $i -lt 15; $i++)
			{
				$bricks = $bricks + (new-object Drawing.Rectangle $brickx, $bricky, 35, 20)
				$brickx = $brickx + 40
			}
			$bricky = $bricky + 30
		}
		
		$bar = new-object Drawing.Rectangle 370,340, 80, 30
		
		#TODO: Place custom script here
		$myBrush = new-object Drawing.SolidBrush black
		$myBrush2 = new-object Drawing.SolidBrush green
		$mypen = new-object Drawing.Pen black
		
		while ($nob -gt 0)
		{
			$Textboxnob.Text = $nob
			$formGraphics.clear("white")
			#Draw bricks
			$i = 0
			$removeBricks = @()
			foreach($brick in $bricks)
			{
				if ($ball.IntersectsWith($brick))
				{
					$score = $score + 10
					$BallSpeedY = + 10
					
					$removeBricks+=$i
				}
				else
				{
					$formGraphics.FillRectangle($myBrush, $brick)
				}
				$i=$i+1
			}
			foreach ($removeBrick in $removeBricks)
			{
				$bricks.remove($bricks[$removeBrick])
			}
			
			#Draw ball
			$formGraphics.FillEllipse($myBrush, $ball)
			
			#Draw bar
			$formGraphics.FillRectangle($myBrush2, $bar) # draw a line	
			
			#move bar
			$bar.X = ([System.Windows.Forms.Cursor]::Position.X) - $form1.Location.X
			
			#check if ball and bar intersects
			if ($ball.IntersectsWith($bar))
			{
				$BallSpeedY = -10		
			}
			$textBoxScore.Text = $score
			
			#check if bar missed ball
			if ($ball.Y -gt 400)
			{
				$nob = $nob - 1
				$ball.X = 100
				$ball.Y = 180
			}
			
			#move ball
			if ($ball.X -gt 620) { $ballSpeedX = -10 }
			if ($ball.X -lt 10) { $ballSpeedX = + 10 }
			$ball.X = $ball.X + $ballSpeedX
			
			if ($ball.Y -gt 450) { $ballSpeedY = -10 }
			if ($ball.Y -lt 70) { $ballSpeedY = + 10 }
			$ball.Y = $ball.Y + $BallSpeedY

			Start-Sleep -m 50
		}
		$Textboxnob.Text = "GAME OVER"
	}
	
$OnLoadForm_StateCorrection=
{#Correct the initial state of the form to prevent the .Net maximized form issue
	$form1.WindowState = $InitialFormWindowState
}

#----------------------------------------------
#region Generated Form Code
$System_Drawing_Size = New-Object System.Drawing.Size
$System_Drawing_Size.Height = 480
$System_Drawing_Size.Width = 640
$form1.ClientSize = $System_Drawing_Size
$form1.DataBindings.DefaultDataSourceUpdateMode = 0
$form1.Name = "form1"
$form1.Text = "Breakout"

$Textboxnob.DataBindings.DefaultDataSourceUpdateMode = 0
$System_Drawing_Point = New-Object System.Drawing.Point
$System_Drawing_Point.X = 370
$System_Drawing_Point.Y = 27
$Textboxnob.Location = $System_Drawing_Point
$Textboxnob.Name = "Textboxnob"
$System_Drawing_Size = New-Object System.Drawing.Size
$System_Drawing_Size.Height = 20
$System_Drawing_Size.Width = 130
$Textboxnob.Size = $System_Drawing_Size
$Textboxnob.TabIndex = 5

$form1.Controls.Add($Textboxnob)

$label2.DataBindings.DefaultDataSourceUpdateMode = 0

$System_Drawing_Point = New-Object System.Drawing.Point
$System_Drawing_Point.X = 280
$System_Drawing_Point.Y = 27
$label2.Location = $System_Drawing_Point
$label2.Name = "label2"
$System_Drawing_Size = New-Object System.Drawing.Size
$System_Drawing_Size.Height = 18
$System_Drawing_Size.Width = 150
$label2.Size = $System_Drawing_Size
$label2.TabIndex = 4
$label2.Text = "Number of balls:"

$form1.Controls.Add($label2)

$label1.DataBindings.DefaultDataSourceUpdateMode = 0

$System_Drawing_Point = New-Object System.Drawing.Point
$System_Drawing_Point.X = 29
$System_Drawing_Point.Y = 25
$label1.Location = $System_Drawing_Point
$label1.Name = "label1"
$System_Drawing_Size = New-Object System.Drawing.Size
$System_Drawing_Size.Height = 23
$System_Drawing_Size.Width = 55
$label1.Size = $System_Drawing_Size
$label1.TabIndex = 3
$label1.Text = "Score:"

$form1.Controls.Add($label1)

$textBoxScore.DataBindings.DefaultDataSourceUpdateMode = 0
$System_Drawing_Point = New-Object System.Drawing.Point
$System_Drawing_Point.X = 90
$System_Drawing_Point.Y = 25
$textBoxScore.Location = $System_Drawing_Point
$textBoxScore.Name = "textBoxScore"
$System_Drawing_Size = New-Object System.Drawing.Size
$System_Drawing_Size.Height = 20
$System_Drawing_Size.Width = 135
$textBoxScore.Size = $System_Drawing_Size
$textBoxScore.TabIndex = 2

$form1.Controls.Add($textBoxScore)


$buttonStop.DataBindings.DefaultDataSourceUpdateMode = 0

$System_Drawing_Point = New-Object System.Drawing.Point
$System_Drawing_Point.X = 40
$System_Drawing_Point.Y = 431
$buttonStop.Location = $System_Drawing_Point
$buttonStop.Name = "buttonStop"
$System_Drawing_Size = New-Object System.Drawing.Size
$System_Drawing_Size.Height = 35
$System_Drawing_Size.Width = 57
$buttonStop.Size = $System_Drawing_Size
$buttonStop.TabIndex = 1
$buttonStop.Text = "Exit"
$buttonStop.UseVisualStyleBackColor = $True
$buttonStop.add_Click($buttonStop_OnClick)

#$form1.Controls.Add($buttonStop)


$buttonStart.DataBindings.DefaultDataSourceUpdateMode = 0

$System_Drawing_Point = New-Object System.Drawing.Point
$System_Drawing_Point.X = 558
$System_Drawing_Point.Y = 431
$buttonStart.Location = $System_Drawing_Point
$buttonStart.Name = "buttonStart"
$System_Drawing_Size = New-Object System.Drawing.Size
$System_Drawing_Size.Height = 35
$System_Drawing_Size.Width = 57
$buttonStart.Size = $System_Drawing_Size
$buttonStart.TabIndex = 0
$buttonStart.Text = "Start"
$buttonStart.UseVisualStyleBackColor = $True
$buttonStart.add_Click($buttonStart_OnClick)

$form1.Controls.Add($buttonStart)

#endregion Generated Form Code

#Save the initial state of the form
$InitialFormWindowState = $form1.WindowState
#Init the OnLoad event to correct the initial state of the form
$form1.add_Load($OnLoadForm_StateCorrection)

#Load the GDI+ and WinForms Assemblies
[reflection.assembly]::LoadWithPartialName( "System.Windows.Forms")
[reflection.assembly]::LoadWithPartialName( "System.Drawing")

# Create pen and brush objects 
$myBrush = new-object Drawing.SolidBrush green
$mypen = new-object Drawing.Pen black


# Get the form's graphics object
$global:formGraphics = $form1.createGraphics()
	
	# Define the paint handler
$form1.add_paint(
{
}
	)
	
$textboxScore.Text="0"
$Textboxnob.Text="3"

#Show the Form
$form1.ShowDialog()| Out-Null
}

#Call the Function
GenerateForm

