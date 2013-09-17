program ExampleCallback;

{$ifdef MSWINDOWS}{$apptype CONSOLE}{$endif}
{$ifdef FPC}{$mode OBJFPC}{$H+}{$endif}

uses
  SysUtils, IPConnection, BrickletMultiTouch;

type
  TExample = class
  private
    ipcon: TIPConnection;
    mt: TBrickletMultiTouch;
  public
    procedure TouchStateCB(sender: TBrickletMultiTouch; const touchState: word);
    procedure Execute;
  end;

const
  HOST = 'localhost';
  PORT = 4223;
  UID = 'XYZ'; { Change to your UID }

var
  e: TExample;

{ Callback function for touch state }
procedure TExample.TouchStateCB(sender: TBrickletMultiTouch; const touchState: word);
var i: integer;
begin
  if (touchState And (1 Shl 12)) = (1 Shl 12) then begin
    Write('In proximity, ');
  end;

  if (touchState And $fff) = 0 then begin
    WriteLn('No electrodes touched');
  end
  else begin
    Write('Electrodes ');
    for i:= 0 to 11 do
	begin
	  if (touchState And (1 Shl i)) = (1 Shl i) then begin
	    Write(IntToStr(i) + ' ');
      end;
	end;
	WriteLn('touched');
  end;
  WriteLn('');
end;

procedure TExample.Execute;
begin
  { Create IP connection }
  ipcon := TIPConnection.Create;

  { Create device object }
  mt := TBrickletMultiTouch.Create(UID, ipcon);

  { Connect to brickd }
  ipcon.Connect(HOST, PORT);
  { Don't use device before ipcon is connected }

  { Register touch state callback to procedure TouchStateCB }
  mt.OnTouchState := {$ifdef FPC}@{$endif}TouchStateCB;

  WriteLn('Press key to exit');
  ReadLn;
  ipcon.Destroy; { Calls ipcon.Disconnect internally }
end;

begin
  e := TExample.Create;
  e.Execute;
  e.Destroy;
end.
