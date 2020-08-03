# RippleEffectInWater
The whole implementation is based on the research paper whose citation is given below:-<br>
X. Zhang and G. Yang, "Ripple simulation based on mass-spring model and image space computation," 2010 3rd International Congress on Image and Signal Processing, Yantai, 2010, pp. 553-557.
<h1>A sketch of "Ripple simulation based on mass-spring model"</h1>
<br>
• The stress and movement of a particle can be calculated fast according to the state of its eight neighbouring paticles. Assuming in a moment, the particle P‘s displacement is S, speed is Vs.
 The stress of particle P can be calculated as:  
                                Fs = k * b * [(B1+B2+B3+B4) - 4 * S ] + k * a * [(A1+A2+A3+A4)-4*S].
<br>•To simulate the deformation effects, the key is to compute the amplitude of each point at each time. We can compute the  the amplitude of a point at the next moment just by certain linear formula according to the 
   amplitudes of its four neighbouring points (the adjacent up, down, left, right points) at current moment given by:
     X’i,j=α·(Xi,j-1+ Xi,j+1+ Xi-1,j Xi+1,j)+b·Xi,j, where the amplitude of Pi,j at t0 is X’i,j; a，b are the influence weights; the amplitude values of Pi,j and its four neighbouring points at moment t0 are Xi,j ，Xi,j-1，Xi,j+1， Xi-1,j and Xi+1,j.<br>
•  On energy conservation we come up to a result with a=1/2 and b=-1<br>
<h2>How does the algorithm works</h2>

<br>• The Width and Lenght of the ocean/water body is defined with value 350(changable).<br>
<br>• The attenuationRate is set to 32 (as mentioned in the above paper).<br>
<br>• A 3-D Array named 'amplitude' is created to store the amplitude at a point Initially and Finally by using the values of 'a' and 'b' as stated above
      . The zeroth index stores the inital amplitude (works as buff1) whereas first index stores the final amplitude (works as buff2) and after creating the ripples 
	there values are swapped. Initially the array is set to 0.
<br><br>• The function calculateAmplitude calculates the amplitude at a point by that from the corresponding 4 points. For the end points (those lying on X=0,Y=0,X=scene-1,Y=scene-1) are calculated by
	their three neighbouring points creating a backward ripple when the initial ripple tounches the boundary. 
<br><br>• A function calculatenormal() is made which computes the normals of the lines drawn between the 3 input points which is further used for creating triangles (by using glNormal3fv) along the normal as to give a 3-D effect. 
	The z coordinate of is calculated by the amplitude factor.
  <br>• The normal are created by calculating the unit vector normal to given 3 points. The triangles are formed by this logic:<br>
If disturbance is added to the RED point, the triangles are formed with the RED, GREEN, GREEN points and the RED, PURPLE, PURPLE points.
Therefore, the triangles formed are the ORANGE and BLUE triangles. This way the ripples are carry fordwarded<br>.
![Alt text](triangles.PNG?raw=true "Title")
<br>
The (red,green,green) points are (j-w, i-w, amplitude[s][j][i],j+1-w, i-w, amplitude[s][j+1][i],j+1-w, i+1-w, amplitude[s][j+1][i+1])) (Here j and i is loop variable, s stand for initial index and w=scene/2).
The (red,purple,purple) points are (j-w, i+1-w, amplitude[s][j][i+1],  j-w, i-w, amplitude[s][j][i], j+1-w, i+1-w, amplitude[s][j+1][i+1]) (Here j and i is loop variable, s stand for initial index and w=scene/2).
Now triangles are drawn in the direction of the normal of these 3 points to give it a sort of ripple effect with the help of these triangles.

  <h2>References</h2>
http://www.glprogramming.com/red/chapter05.html<br>
https://community.khronos.org/t/implement-water-ripple-effect/72779
