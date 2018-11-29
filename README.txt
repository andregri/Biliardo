How to hide the cursor and create a "texture cursor"

The system function ShowCursor() show of hide the cursor. 
But if you hide the cursor you cannot reach the commands on the window bar or resize the window.
The solution is to change the cursor status as follows
(in the callback function)

case WM_MOUSEMOVE:
      {
         POINTS p;
         p = MAKEPOINTS(lParam);
         Data.cx = p.x; Data.cy = p.y;
         if( Data.IsInClient(p.x,p.y) ) {
           if( !Data.captured ) { Data.captured = true; SetCapture(hWnd); ShowCursor(FALSE); }
         } else {
           if( Data.captured ) { Data.captured = false; ReleaseCapture(); ShowCursor(TRUE); }
         }
      }
      break;

See also the code in the MyModel::DrawGLScene() function.