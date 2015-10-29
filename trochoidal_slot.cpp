#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <getopt.h>
#include <string.h>

using namespace std;

struct point
{
	double x;
	double y;
};

int main(int argc, char**argv)
{
	int notEnoughParametersFlag = 0;

	point startPoint;
	point endPoint;

	startPoint.x = 2.5;
	startPoint.y = -2.75;
	int startPointFlag = 0;

	endPoint.x = 2.5;
	endPoint.y = 0.1;
	int endPointFlag = 0;

	double slotWidth = 2.020;
	int slotWidthFlag = 0;

	double feedRate = 14.0;
	int feedRateFlag = 0;

	double returnFeedRate = 50.0;  //Non cutting side of trochoid.
	int returnFeedRateFlag = 0;

	double stepOver = .100;
	int stepOverFlag = 0;
	
	double toolDiameter = 1.250;
	int toolDiameterFlag = 0;

	double safeZ;
	int safeZFlag = 0;

	double zDepth;
	int zDepthFlag = 0;



	int c;
	while(1)
	{
		static struct option long_options[] =
		{
		/*These option do not set a flag.  We distinguish them by their indices. */
			{"width",	required_argument, 0, 'w' },
			{"feed",	required_argument, 0, 'f' },
			{"return",	optional_argument, 0, 'r' },
			{"stepover",	required_argument, 0, 's' },
			{"tooldiameter",required_argument, 0, 't' },
			{"begin",	required_argument, 0, 'b' },
			{"end",		required_argument, 0, 'e' },
			{"zdepth",	required_argument, 0, 'z' },
			{"safez",	required_argument, 0, 'x' },
			{0,0,0,0}
		};

		int option_index = 0;

		c = getopt_long( argc, argv, "w:f:r:s:t:b:e:z:x:", long_options, &option_index );

		if( c == -1 )
			break;

		switch( c )
		{
			case 'w':
				//printf ("option -w with value %s\n", optarg);
				if( optarg != NULL )
				{
					slotWidth = atof( optarg );
					slotWidthFlag = 1;
				}
				break;

			case 'f':
				//printf ("option -f with value %s\n", optarg);
				if( optarg != NULL )
				{
					feedRate = atof( optarg );
					feedRateFlag = 1;
				}
				break;

			case 'r':
				//printf ("option -r with value %s\n", optarg);
				if( optarg != NULL )
				{
					returnFeedRate = atof( optarg );
					returnFeedRateFlag = 1;
				}
				break;

			case 's':
				//printf ("option -s with value %s\n", optarg);
				if( optarg != NULL )
				{
					stepOver = atof( optarg );
					stepOverFlag = 1;
				}
				break;

			case 't':
				//printf ("option -t with value %s\n", optarg);
				if( optarg != NULL )
				{
					toolDiameter = atof( optarg );
					toolDiameterFlag = 1;
				}
				break;

			case 'b':
				//Parse starting point
				//printf ("option -b with value %s\n", optarg);
				{				
					char* tempstr = new char[strlen(optarg)];
					strcpy( tempstr, optarg );
				
					int i = 0;
					char *pch = strtok( tempstr,"," );
					for( ;					     
					     i < 2 && pch != NULL;
					     ++i )
					{
					
						if( i == 0 )
						{	startPoint.x = atof( pch );	++startPointFlag; }
						if( i == 1 )
						{	startPoint.y = atof( pch );	++startPointFlag; }
					
						pch = strtok( NULL, "," );
					}
				
					delete tempstr;
				}				
				break;

			case 'e':
				//Parse ending point
				//printf ("option -e with value %s\n", optarg);
				{				
					char* tempstr = new char[strlen(optarg)];
					strcpy( tempstr, optarg );
				
					int i = 0;
					char *pch = strtok( tempstr,"," );
					for( ;					     
					     i < 2 && pch != NULL;
					     ++i )
					{
					
						if( i == 0 )
						{	endPoint.x = atof( pch );	++endPointFlag; }
						if( i == 1 )
						{	endPoint.y = atof( pch );	++endPointFlag; }
					
						pch = strtok( NULL, "," );
					}
				
					delete tempstr;
				}				
				break;
			
			case 'z':				
				if( optarg != NULL )
				{
					zDepth = atof( optarg );
					zDepthFlag = 1;
				}
				break;

			case 'x':
				if( optarg != NULL )
				{
					safeZ = atof( optarg );
					safeZFlag = 1;
				}
				break;
			
		}

		
	}

	//Check required parameters have been specified.
	if( startPointFlag != 2 )
	{	cerr << "Start point not specified or invalid." << endl;	notEnoughParametersFlag = 1; }
	if( endPointFlag != 2 )
	{	cerr << "End point not specified or invalid." << endl;		notEnoughParametersFlag = 1; }
	if( slotWidthFlag != 1 )
	{	cerr << "Slot width not specified." << endl;			notEnoughParametersFlag = 1; }
	if( toolDiameterFlag != 1 )
	{	cerr << "Tool diameter not specified." << endl;			notEnoughParametersFlag = 1; }
	if( feedRateFlag != 1 )
	{	cerr << "Feed rate not specified." << endl;			notEnoughParametersFlag = 1; }
	if( stepOverFlag != 1 )
	{	cerr << "Stepover not specified." << endl;			notEnoughParametersFlag = 1; }
	if( zDepthFlag != 1 )
	{	cerr << "Z depth not specified." << endl;			notEnoughParametersFlag = 1; }
	if( safeZFlag != 1 )
	{	cerr << "Safe Z not specified." << endl;			notEnoughParametersFlag = 1; }

	if( notEnoughParametersFlag > 0 )
	{
		cerr << "*** Not enough parameters specified to generate tool path. ***" << endl;
		return 1;
	}

	double trocoidRadius = ( slotWidth - toolDiameter ) / 2;
	double slotAngle;

        double xDiff = endPoint.x - startPoint.x;
        double yDiff = endPoint.y - startPoint.y; 

	slotAngle = atan2( yDiff, xDiff);
	cout << "(Slot Angle deg:  " << (slotAngle / M_PI) * 180 << ")" << endl;
	cout << "(Start Point: " << startPoint.x << ", " << startPoint.y << " )" << endl;
	cout << "(End Point:   " << endPoint.x << ", " << endPoint.y << " )" << endl;
	cout << "(Slot Width:  " << slotWidth << " )" << endl;
	cout << "(Step Over:   " << stepOver << " )" << endl;
	point currentPoint;
	point nextPoint;

	currentPoint.x = startPoint.x;
	currentPoint.y = startPoint.y;

	cout.precision(7);
	cout << "G40" << endl;	
	
	//cout << "T1 M6" << endl;


	//Go to start point.
	cout << "G0 X" << startPoint.x << " Y" << startPoint.y << endl;
	cout << "G0 Z" << zDepth << endl; 

	//Turn on cutter compensation (left) G41
	//cout << "G41" << endl;

	point startEntryPoint;
        startEntryPoint.x = currentPoint.x + ( trocoidRadius * cos( slotAngle - M_PI_2 ) );
        startEntryPoint.y = currentPoint.y + ( trocoidRadius * sin( slotAngle - M_PI_2 ) );

	cout << "G1 F" << feedRate << " X" << fixed << startEntryPoint.x << " Y" << fixed << startEntryPoint.y << endl;
	


	while( !(currentPoint.x == endPoint.x && currentPoint.y == endPoint.y) )
	{
		point entryPoint;
		point exitPoint;

		entryPoint.x = currentPoint.x + ( trocoidRadius * cos( slotAngle - M_PI_2 ) );
		entryPoint.y = currentPoint.y + ( trocoidRadius * sin( slotAngle - M_PI_2 ) );

		exitPoint.x = currentPoint.x + ( trocoidRadius * ( cos( slotAngle + M_PI_2 ) ) );
		exitPoint.y = currentPoint.y + ( trocoidRadius * ( sin( slotAngle + M_PI_2 ) ) );

		//Arc to exit point.
		cout << "G3 F" << feedRate << " X" << exitPoint.x << " Y" << exitPoint.y
						<< " I" << currentPoint.x - entryPoint.x
						<< " J" << currentPoint.y - entryPoint.y
						<<  endl;


		nextPoint.x = currentPoint.x + ( stepOver * cos(slotAngle) );
		nextPoint.y = currentPoint.y + ( stepOver * sin(slotAngle) );
		
		entryPoint.x = nextPoint.x + ( trocoidRadius * cos( slotAngle - M_PI_2 ) );
		entryPoint.y = nextPoint.y + ( trocoidRadius * sin( slotAngle - M_PI_2 ) );
		//Arc to next entry point.
                cout << "G3 F" << returnFeedRate << " X" << entryPoint.x << " Y" << entryPoint.y 
	                                        << " I" << (entryPoint.x - exitPoint.x) / 2
                                                << " J" << (entryPoint.y - exitPoint.y) / 2
                                                <<  endl;

		currentPoint.x = nextPoint.x;
		currentPoint.y = nextPoint.y;
		

		//Check distance to end point.
		xDiff = endPoint.x - currentPoint.x;
		yDiff = endPoint.y - currentPoint.y;

		double DTE =  sqrt( (xDiff * xDiff ) + (yDiff * yDiff ) );

		if( DTE < stepOver )
		{
			break;
		}
		
	}

	cout << "G0 X" << endPoint.x << " Y" << endPoint.y << endl; 
	cout << "G0 Z" << safeZ << endl;


	cout << endl << "M2" << endl;


	return 0;
}
