#include <windows.h>


bool is_own_console()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    if ( !GetConsoleScreenBufferInfo( GetStdHandle( STD_OUTPUT_HANDLE ), &csbi ) )
    {
        printf( "GetConsoleScreenBufferInfo failed: %lu\n", GetLastError() );
        return FALSE;
    }

    // if cursor position is (0,0) then we were launched in a separate console
    return ( ( !csbi.dwCursorPosition.X ) && ( !csbi.dwCursorPosition.Y ) );
}

void wait_keyboard()
{
    while ( !_kbhit() )
    {
        Sleep( 100 );
    }
    _getch();
}

