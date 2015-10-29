#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <getopt.h>
#include <string.h>


struct point
{
	double x;
	double y;
};

int main(int argc, char**argv)
{

	point startPoint;
	point endPoint;

	startPoint.x = 2.5;
	startPoint.y = -2.75;

	endPoint.x = 2.5;
	endPoint.y = 0.1;

	double slotWidth = 2.020;
	double feedRate = 14.0;
	double returnFeedRate = 50.0;  //Non cutting side of trochoid.
	double stepOver = .100;
	double toolDiameter = 1.250;
	double safeZ;

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
			{0,0,0,0}
		};

		int option_index = 0;

		c = getopt_long( argc, argv, "w:f:r:s:t:b:e:", long_options, &option_index );

		if( c == -1 )
			break;

		switch( c )
		{
			case 'w':
				printf ("option -w with value %s\n", optarg);
				slotWidth = atof( optarg );
				break;

			case 'f':
				printf ("option -f with value %s\n", optarg);
				feedRate = atof( optarg );
				break;

			case 'r':
				printf ("option -r with value %s\n", optarg);
				returnFeedRate = atof( optarg );
				break;

			case 's':
				printf ("option -s with value %s\n", optarg);
				stepOver = atof( optarg );
				break;

			case 't':
				printf ("option -t with value %s\n", optarg);
				toolDiameter = atof( optarg );
				break;

			case 'b':
				//Parse starting point
				printf ("option -b with value %s\n", optarg);
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
							startPoint.x = atof( pch );
						if( i == 1 )
							startPoint.y = atof( pch );
					
						pch = strtok( NULL, "," );
					}
				
					delete tempstr;
				}				
				break;

			case 'e':
				//Parse ending point
				printf ("option -e with value %s\n", optarg);
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
							endPoint.x = atof( pch );
						if( i == 1 )
							endPoint.y = atof( pch );
					
						pch = strtok( NULL, "," );
					}
				
					delete tempstr;
				}				
				break;
			
		}

		
	}

	double trocoidRadius = ( slotWidth - toolDiameter ) / 2;
	double slotAngle;

        double xDiff = endPoint.x - startPoint.x;
        double yDiff = endPoint.y - startPoint.y; 

	slotAngle = atan2( yDiff, xDiff);
	std::cout << "(Slot Angle deg:  " << (slotAngle / M_PI) * 180 << ")" << std::endl;
	std::cout << "(Start Point: " << startPoint.x << ", " << startPoint.y << " )" << std::endl;
	std::cout << "(End Point:   " << endPoint.x << ", " << endPoint.y << " )" << std::endl;
	std::cout << "(Slot Width:  " << slotWidth << " )" << std::endl;
	std::cout << "(Step Over:   " << stepOver << " )" << std::endl;
	point currentPoint;
	point nextPoint;

	currentPoint.x = startPoint.x;
	currentPoint.y = startPoint.y;

	std::cout.precision(7);
	std::cout << "G40" << std::endl;	
	std::cout << "T1 M6" << std::endl;


	//Perform lead in move.  
	std::cout << "G0 X" << startPoint.x << " Y" << startPoint.y << std::endl; 

	//Turn on cutter compensation (left) G41
	//std::cout << "G41" << std::endl;

	point startEntryPoint;
        startEntryPoint.x = currentPoint.x + ( trocoidRadius * cos( slotAngle - M_PI_2 ) );
        startEntryPoint.y = currentPoint.y + ( trocoidRadius * sin( slotAngle - M_PI_2 ) );

	std::cout << "G1 F" << feedRate << " X" << std::fixed << startEntryPoint.x << " Y" << std::fixed << startEntryPoint.y << std::endl;
	


	while( !(currentPoint.x == endPoint.x && currentPoint.y == endPoint.y) )
	{
		point entryPoint;
		point exitPoint;

		entryPoint.x = currentPoint.x + ( trocoidRadius * cos( slotAngle - M_PI_2 ) );
		entryPoint.y = currentPoint.y + ( trocoidRadius * sin( slotAngle - M_PI_2 ) );

		exitPoint.x = currentPoint.x + ( trocoidRadius * ( cos( slotAngle + M_PI_2 ) ) );
		exitPoint.y = currentPoint.y + ( trocoidRadius * ( sin( slotAngle + M_PI_2 ) ) );

		//Arc to exit point.
		std::cout << "G3 F" << feedRate << " X" << exitPoint.x << " Y" << exitPoint.y
						<< " I" << currentPoint.x - entryPoint.x
						<< " J" << currentPoint.y - entryPoint.y
						<<  std::endl;


		nextPoint.x = currentPoint.x + ( stepOver * cos(slotAngle) );
		nextPoint.y = currentPoint.y + ( stepOver * sin(slotAngle) );
		
		entryPoint.x = nextPoint.x + ( trocoidRadius * cos( slotAngle - M_PI_2 ) );
		entryPoint.y = nextPoint.y + ( trocoidRadius * sin( slotAngle - M_PI_2 ) );
		//Arc to next entry point.
                std::cout << "G3 F" << returnFeedRate << " X" << entryPoint.x << " Y" << entryPoint.y 
	                                        << " I" << (entryPoint.x - exitPoint.x) / 2
                                                << " J" << (entryPoint.y - exitPoint.y) / 2
                                                <<  std::endl;

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

	std::cout << "G0 X" << endPoint.x << " Y" << endPoint.y << std::endl; 	
	//std::cout << "G40" << std::endl;
	std::cout << "M2" << std::endl;


	return 0;
}
