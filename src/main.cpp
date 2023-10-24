#include <PdfGen.h>

#include <unistd.h>

void printUsage( int argc, char* argv[] )
{
  printf("Usage: %s [-a] input.pnm output.pdf\n", argv[0]);
  exit(0);
}

int main( int argc, char* argv[] )
{
  bool header = false;
  if (argc < 3)
	  printUsage( argc, argv );

  int c;
  while ( (c = getopt(argc-2, argv, "a") ) != -1 )
  {
    switch(c)
    {
    case 'a':
      header = true;
      break;
    }
  }

  FILE* f = fopen( argv[argc-2], "rb");
  if (!f)
  {
    printf("File not found: %s\n", argv[1]);
    return -1;
  }
  fseek( f, 0, SEEK_END );
  size_t size = ftell(f);
  rewind(f);
  std::vector<uint8_t> buffer(size);
  fread( buffer.data(), 1, size, f );

  pdfgen::PdfGen pg;

  pg.AddPage();
  if ((buffer.size() > 10) && (buffer[1] == 'P') && (buffer[2] == 'N') && (buffer[3] == 'G'))
    pg.DrawPngImage( buffer );
  else
    pg.DrawPbmImage( buffer );
  if (header)
    pg.DrawText(0,0,"Scanned by: Yves De Muyter");

  pg.Save( std::string(argv[argc-1]) );

  return 0;
}
