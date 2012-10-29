#include<stdlib.h>
#include<iostream>
#include<time.h>
#include<glut.h>	
#include<stdio.h>
#include<conio.h>
using namespace std;

void initialise();//initialises random value to the matrix
void normalkeymapping(unsigned char k,int x,int y);//to get normal keys from the keyboard
void specialkeymapping(int k,int x,int y);//to get keys like up arrow, down arrow, etc.
void navigation(int ,int );//for moving the location pointer to the present box
void selection(int ,int);//for moving the location pointer to the present box during selection
void updation();//for updating the matrix after removing a group of matrix
int check();// to check whether any more moves are left for the player
void rendering();//Initializes 3D rendering
void resizingwindow(int w, int h);//Called when the window is resized
void drawing();//Draws the 3D scene

int gridcl[10][10];//holds the color of the blocks(cells) in the grid GREEN=0 RED=1 NOBLOCK=-1 
int columnsize[10],rowsize=9;
int sel_locationi[100],sel_locationj[100],sel_counter,checker;//for storing present and selected location
void initialise()
{
	int i,j,checker=-1;rowsize=9;
	sel_locationi[0]=0;sel_locationj[0]=0;sel_counter=0;
	srand(time(NULL));
	for(j=0;j<=9;j+=1)
		{
			columnsize[j]=rand()%10;
			for(i=0;i<=9;i+=1.0)
			{	
				if( i> columnsize[j]) gridcl[i][j]=-1;
				else gridcl[i][j]=rand()%2;
			}
	}
}
void normalkeymapping(unsigned char k,int x,int y)
{
	int counter;
	switch(k)
	{
		case 27: //Escape key
			exit(0);break;
		case 13: //Enter key
			if(sel_counter>=2) 
			{
				for(counter=sel_counter;counter>=0;counter--)
				{
					gridcl[sel_locationi[counter]][sel_locationj[counter]]=-1;
				}
				updation();
			}
			break;
		case 'n': initialise();glutPostRedisplay();cout<<"\n       NEW GAME\n";break;
	}
}
void specialkeymapping(int k,int x,int y)
{
	int i=0,j=0;
	switch(k)
	{
	case GLUT_KEY_LEFT:
		j=-1;break;
	case GLUT_KEY_RIGHT:
		j=1;break;
	case GLUT_KEY_UP:
		i=1;break;
	case GLUT_KEY_DOWN:
		i=-1;break;
	}
	int mod= glutGetModifiers();
	if (mod == GLUT_ACTIVE_CTRL) selection(i,j); 
	else navigation(i, j);
}
void navigation(int i,int j)
{
	int y=sel_locationi[sel_counter],x=sel_locationj[sel_counter];
	if(i!=0)
	{
		y=y+i;
		if(y>columnsize[x]) y=0;
		else if(y<0) y=columnsize[x];
	}
	if(j!=0)
	{
		do
		{ 
			x+=j;
			if(x>rowsize)x=0;else if(x<0)x=rowsize;
		}while(y>columnsize[x]);
	}
	sel_counter=0;
	sel_locationi[sel_counter]=y;sel_locationj[sel_counter]=x;
	glutPostRedisplay();
}
void selection(int i,int j)
{
	
	int y=sel_locationi[sel_counter],x=sel_locationj[sel_counter],mov=0,counter;
	if(i!=0)
	{
		if((y+i)<=columnsize[x] && (y+i)>=0) 
		{
			y+=i;mov=1;
		}
	}
	if(j!=0)
	{
		if((x+j)<=rowsize && (x+j)>=0 && y<=columnsize[x+j]) 
		{
			x+=j;mov=1;
		}
	}
	for(counter=sel_counter;counter>=0;counter--)
		if(x==sel_locationj[counter] && y==sel_locationi[counter]) mov=0;
	if(gridcl[y][x] == gridcl[sel_locationi[sel_counter]][sel_locationj[sel_counter]] && mov==1) 
	{
		sel_counter++;
		sel_locationi[sel_counter]=y;sel_locationj[sel_counter]=x;
		glutPostRedisplay();
	}
}
void updation()
{
	int i,j,k;
	for(j=0;j<=rowsize;j++)
	{
		for(i=0;i<=columnsize[j];i++)
		{ 
			if(gridcl[i][j]==-1) 
			{
				for(k=i+1;k<=columnsize[j];k++)
				{
					gridcl[k-1][j]=gridcl[k][j];
					gridcl[k][j]=-1;
				}
				columnsize[j]--;i--;
			}
		}
		if(columnsize[j]==-1)
		{
			for(k=j+1;k<=rowsize;k++)
			{
				for(i=0;i<=columnsize[k];i++)
				{
					gridcl[i][k-1]=gridcl[i][k];
					gridcl[i][k]=-1;
				}
				columnsize[k-1]=columnsize[k];columnsize[k]=-1;
			}
			rowsize--;j--;	
		}
	}
	sel_locationi[0]=0;sel_locationj[0]=0;sel_counter=0;
	if(gridcl[0][0]==-1) cout<<"       ************** YOU ********* WON *****************\n\n ";
	else if(check()==0)  cout<<"       ************** YOU ********* LOST *****************\n\n ";
		
	glutPostRedisplay();

	
}
int check()
{
	int i,j,val,x,y,x1,y1,i1,j1;

	for(j=0;j<=rowsize;j++)
	{
		for(i=0;i<=columnsize[j];i++)
		{ 
			val=gridcl[i][j];
			for(int ch=1;ch<=4;ch++)
			{
				if(ch==1){ x=0;y=1;}
				else if(ch==2) {x=-1;y=0;}
				else if(ch==3) {x=0;y=-1;}
				else if(ch==4) {x=1;y=0;}
			
				if((i+y)>=0 && (j+x)>=0 && (i+y)<=columnsize[j+x] && (j+x)<=rowsize && gridcl[i][j]==gridcl[i+y][j+x])
				{
					for(int ch1=1;ch1<=4;ch1++)
					{
						if(ch1==1){ x1=0;y1=1;}
						else if(ch1==2) {x1=-1;y1=0;}
						else if(ch1==3) {x1=0;y1=-1;}
						else if(ch1==4) {x1=1;y1=0;}
						i1=i+y+y1;j1=j+x+x1;
						if((i1)>=0 && (j1)>=0 && (i1)<=columnsize[j1] && (j1)<=rowsize && gridcl[i][j]==gridcl[i1][j1] && (x!=-x1 ||x==0) && (y!=-y1 ||y==0))
							return -1;
					}
				}
			}
		}
	}
	return 0;
}
void rendering()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
}
void resizingwindow(int w, int h)
{
	glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45,w/h,1,2000);
}
void drawing()
{
	
	int i,j,counter;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	for(j=0;j<=rowsize;j+=1)
		{
			for(i=0;i<=columnsize[j];i+=1)
			{
				glPushMatrix();
				glTranslatef(j-4.5,i-4.5,-15.0f);
				if(gridcl[i][j]==0)glColor3f(0.0,1.0,0.0);
				else if(gridcl[i][j]==1) glColor3f(1.0,0.0,0.0);
				else if(gridcl[i][j]==-1) glColor3f(0.0,0.0,0.0);
				glBegin(GL_QUADS);
				glVertex3f(-0.45f,-0.45f,0.0f);
				glVertex3f(-0.45f,0.45f,0.0f);
				glVertex3f(0.45f,0.45f,0.0f);
				glVertex3f(0.45f,-0.45f,0.0f);
				glEnd();
				glPopMatrix();
			}
	}
	for(counter=sel_counter;counter>=0;counter--)
	{
		glPushMatrix();
		glTranslatef(sel_locationj[counter]-4.5,sel_locationi[counter]-4.5,-15.01f);
		glColor3f(1.0,1.0,1.0);
		glBegin(GL_QUADS);
		glVertex3f(-0.5f,-0.5f,0.0f);
		glVertex3f(-0.5f,0.5f,0.0f);
		glVertex3f(0.5f,0.5f,0.0f);
		glVertex3f(0.5f,-0.5f,0.0f);
		glEnd();
		glPopMatrix();
	}
	
	glutSwapBuffers();
}
int main(int argc,char **argv)
{
	initialise();
	cout<<"CONTROLS\nUse arrow keys to move across the blocks.\nUSe Ctrl+Arrow keys for selecting blocks\n";
	cout<<"Hit Enter for removing the selected 3 or more blocks\n";
	cout<<"Press n for NEW GAME\nPress Esp for exit\n";
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(400, 400); //Set the window size
	glutInitWindowPosition(700,300);
	glutCreateWindow("TETRIS");
	rendering();

	glutDisplayFunc(drawing);
	glutKeyboardFunc(normalkeymapping);
	glutSpecialFunc(specialkeymapping);
	glutReshapeFunc(resizingwindow);

	glutMainLoop();

	return 0;
}