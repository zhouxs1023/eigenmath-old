#include "stdafx.h"

#ifdef MAC
#define EOL "\n"
#else
#define EOL "\r\n"
#endif

char *example_script[7] = {

// 1. GammaMatrixAlgebra

"# This script does a few of the exercises from Feynman's book \"Quantum Electrodynamics.\""EOL
"# To run this script, click on the 'Run Script' button (in the main window)."EOL
""EOL
"# Define the spacetime metric (for multiplying spacetime vectors)."EOL
""EOL
"metric = ((-1, 0, 0, 0),"EOL
"          ( 0,-1, 0, 0),"EOL
"          ( 0, 0,-1, 0),"EOL
"          ( 0, 0, 0, 1))"EOL
""EOL
"# Define I, the identity matrix."EOL
""EOL
"I = ((1,0,0,0),(0,1,0,0),(0,0,1,0),(0,0,0,1))"EOL
""EOL
"# Define the gamma matrices."EOL
""EOL
"gammax = (( 0, 0, 0, 1),"EOL
"          ( 0, 0, 1, 0),"EOL
"          ( 0,-1, 0, 0),"EOL
"          (-1, 0, 0, 0))"EOL
""EOL
"gammay = (( 0, 0, 0,-i),"EOL
"          ( 0, 0, i, 0),"EOL
"          ( 0, i, 0, 0),"EOL
"          (-i, 0, 0, 0))"EOL
""EOL
"gammaz = (( 0, 0, 1, 0),"EOL
"          ( 0, 0, 0,-1),"EOL
"          (-1, 0, 0, 0),"EOL
"          ( 0, 1, 0, 0))"EOL
""EOL
"gammat = (( 1, 0, 0, 0),"EOL
"          ( 0, 1, 0, 0),"EOL
"          ( 0, 0,-1, 0),"EOL
"          ( 0, 0, 0,-1))"EOL
""EOL
"# Define the gamma vector."EOL
"#"EOL
"# The gamma vector has gamma matrices for its components. We express it here"EOL
"# as a rank 3 tensor. We set up the tensor so that the vector component index"EOL
"# is the last (rightmost) index. With this configuration we can left-multiply"EOL
"# with a Feynman slash matrix using the dot function."EOL
"#"EOL
"# For example, in component notation, this is how we want to multiply with a"EOL
"# Feynman slash matrix:"EOL
"#"EOL
"#     aslash[a,b] gamma[b,c,d]"EOL
"#"EOL
"# (summation over the repeated index b)"EOL
"#"EOL
"# The summation over b is exactly what the dot function does so we can do the"EOL
"# above multiply with dot(aslash,gamma)."EOL
"#"EOL
"# In the following outer products, placing the basis vector operands on the"EOL
"# right-hand side results in the desired index ordering."EOL
""EOL
"gamma = outer(gammax,(1,0,0,0)) +"EOL
"        outer(gammay,(0,1,0,0)) +"EOL
"        outer(gammaz,(0,0,1,0)) +"EOL
"        outer(gammat,(0,0,0,1))"EOL
""EOL
"# DOT is for multiplying gamma vectors. This is a special multiply because we"EOL
"# have to dot the individual vector components (the gamma matrices) then we"EOL
"# have to sum over all the results. In component notation, this is how we want"EOL
"# to do the multiply:"EOL
"#"EOL
"#     T[a,c] = A[a,b,d] B[b,c,d]"EOL
"#"EOL
"# To do this, we start with an outer product which results in the following"EOL
"# rank 6 tensor:"EOL
"#"EOL
"#     T[a,b,d,b,c,d]"EOL
"#"EOL
"# Next we sum over b (indices 2 and 4) to get the following:"EOL
"#"EOL
"#     T[a,d,c,d]"EOL
"#"EOL
"# Then we sum over d (indices 2 and 4 again) to get"EOL
"#"EOL
"#     T[a,c]"EOL
"#"EOL
"# One final note, dot(B,metric) applies the spacetime metric to the rightmost"EOL
"# index of B, the vector index."EOL
""EOL
"DOT(A,B) = contract(contract(outer(A,dot(B,metric)),2,4),2,4)"EOL
""EOL
"# Define arbitrary spacetime vectors a, b and c."EOL
""EOL
"a = (ax,ay,az,at)"EOL
"b = (bx,by,bz,bt)"EOL
"c = (cx,cy,cz,ct)"EOL
""EOL
"# Define generic Feynman slash matrices."EOL
"# Note: The order of dot operands here is different from the book. This is"EOL
"# because we defined gamma to have its vector index on the right. Therefore"EOL
"# we have to right-multiply with the spacetime vector so that dot contracts"EOL
"# over the correct indices. In component notation we have"EOL
"#"EOL
"#     aslash[u,v] = gamma[u,v,w] a[w]"EOL
"#"EOL
"# where summation is over the repeated index w."EOL
""EOL
"aslash = dot(gamma,metric,a)"EOL
"bslash = dot(gamma,metric,b)"EOL
"cslash = dot(gamma,metric,c)"EOL
""EOL
"# The Feynman slash matrices are 4x4 matrices. For example, aslash looks like"EOL
"# this:"EOL
"#"EOL
"#     at           0           -az       -ax + i ay"EOL
"#"EOL
"#     0            at       -ax - i ay       az"EOL
"#"EOL
"#     az       ax - i ay       -at           0"EOL
"#"EOL
"# ax + i ay       -az           0           -at"EOL
""EOL
"# Now we are ready to try the exercises. We want to show that each of the"EOL
"# following identities is true."EOL
""EOL
"\"Checking the following identities:\""EOL
""EOL
"#------------------------------------------------------------------------------"EOL
"#"EOL
"#     aslash = at gammat - ax gammax - ay gammay - az gammaz"EOL
"#"EOL
"#------------------------------------------------------------------------------"EOL
""EOL
"display(quote(aslash = at gammat - ax gammax - ay gammay - az gammaz))"EOL
""EOL
"A = aslash"EOL
""EOL
"B = at gammat - ax gammax - ay gammay - az gammaz"EOL
""EOL
"check(A - B) # if A - B = 0 then continue, else stop"EOL
""EOL
"#------------------------------------------------------------------------------"EOL
"#"EOL
"#     aslash bslash = -bslash aslash + 2 a b"EOL
"#"EOL
"#------------------------------------------------------------------------------"EOL
""EOL
"display(quote(aslash bslash = -bslash aslash + 2 a b))"EOL
""EOL
"A = dot(aslash,bslash)"EOL
""EOL
"B = -dot(bslash,aslash) + 2 dot(a,metric,b) I"EOL
""EOL
"check(A - B)"EOL
""EOL
"#------------------------------------------------------------------------------"EOL
"#"EOL
"#     gamma gamma = 4"EOL
"#"EOL
"#------------------------------------------------------------------------------"EOL
""EOL
"display(quote(gamma gamma = 4))"EOL
""EOL
"A = DOT(gamma,gamma)"EOL
""EOL
"B = 4 I"EOL
""EOL
"check(A - B)"EOL
""EOL
"#------------------------------------------------------------------------------"EOL
"#"EOL
"#     gamma aslash gamma = -2 aslash"EOL
"#"EOL
"#------------------------------------------------------------------------------"EOL
""EOL
"display(quote(gamma aslash gamma = -2 aslash))"EOL
""EOL
"A = DOT(gamma,dot(aslash,gamma))"EOL
""EOL
"B = -2 aslash"EOL
""EOL
"check(A - B)"EOL
""EOL
"#------------------------------------------------------------------------------"EOL
"#"EOL
"#     gamma aslash bslash gamma = 4 a b"EOL
"#"EOL
"#------------------------------------------------------------------------------"EOL
""EOL
"display(quote(gamma aslash bslash gamma = 4 a b))"EOL
""EOL
"A = DOT(gamma,dot(aslash,bslash,gamma))"EOL
""EOL
"B = 4 dot(a,metric,b) I"EOL
""EOL
"check(A - B)"EOL
""EOL
"#------------------------------------------------------------------------------"EOL
"#"EOL
"#     gamma aslash bslash cslash gamma = -2 cslash bslash aslash"EOL
"#"EOL
"#------------------------------------------------------------------------------"EOL
""EOL
"display(quote(gamma aslash bslash cslash gamma = -2 cslash bslash aslash))"EOL
""EOL
"A = DOT(gamma,dot(aslash,bslash,cslash,gamma))"EOL
""EOL
"B = -2 dot(cslash,bslash,aslash)"EOL
""EOL
"check(A - B)"EOL
""EOL
"#------------------------------------------------------------------------------"EOL
"#"EOL
"#     If we get here then everything worked."EOL
"#"EOL
"#------------------------------------------------------------------------------"EOL
""EOL
"\"OK\""EOL,

// 2. VectorCalculus

"# This script tests 10 vector calculus identities."EOL
""EOL
"# Define the cross product, div, grad, curl and laplacian for"EOL
"# rectangular coordinates."EOL
""EOL
"cross(u,v) = ("EOL
"  u[2] v[3] - u[3] v[2],"EOL
"  u[3] v[1] - u[1] v[3],"EOL
"  u[1] v[2] - u[2] v[1]"EOL
")"EOL
""EOL
"div(v) = contract(d(v,(x,y,z)),1,2)"EOL
""EOL
"grad(v) = d(v,(x,y,z))"EOL
""EOL
"curl(f) = ("EOL
"  d(f[3],y) - d(f[2],z),"EOL
"  d(f[1],z) - d(f[3],x),"EOL
"  d(f[2],x) - d(f[1],y)"EOL
")"EOL
""EOL
"laplacian(f) = d(d(f,x),x) + d(d(f,y),y) + d(d(f,z),z)"EOL
""EOL
"# Note: Functions can be left undefined, such as FX(), FY(), etc."EOL
"# These \"generic\" functions, when evaluated by the derivative function d(),"EOL
"# are considered to be dependent on all variables."EOL
"# Basically what this means is that d() does no evaluation at all."EOL
"# For example, d(FX(),x) returns the expression d(FX(),x)."EOL
""EOL
"# Define generic vector functions F and G."EOL
""EOL
"F = (FX(),FY(),FZ())"EOL
"G = (GX(),GY(),GZ())"EOL
""EOL
"# Now check the 10 identities."EOL
""EOL
"\"Checking the following identities:\""EOL
""EOL
"\"1. div(curl F) = 0\""EOL
""EOL
"A = div(curl(F))"EOL
""EOL
"check(A)"EOL
""EOL
"\"2. curl(grad f) = 0\""EOL
""EOL
"A = curl(grad(f())) # Note the use of generic scalar function f() here."EOL
""EOL
"check(A)"EOL
""EOL
"\"3. div(grad f) = laplacian f\""EOL
""EOL
"A = div(grad(f()))"EOL
""EOL
"B = laplacian(f())"EOL
""EOL
"check(A - B)"EOL
""EOL
"\"4. curl(curl F) = grad(div F) - laplacian F\""EOL
""EOL
"A = curl(curl(F))"EOL
""EOL
"B = grad(div(F)) - laplacian(F)"EOL
""EOL
"check(A - B)"EOL
""EOL
"\"5. grad(fg) = f grad g + g grad(f)\""EOL
""EOL
"A = grad(f() g())"EOL
""EOL
"B = f() grad(g()) + g() grad(f())"EOL
""EOL
"check(A - B)"EOL
""EOL
"\"6. grad(F . G) = (G . grad)F + (F . grad)G + G x curl F + F x curl G\""EOL
""EOL
"A = grad(dot(F,G))"EOL
""EOL
"B = dot(grad(F),G)+dot(grad(G),F)+cross(G,curl(F))+cross(F,curl(G))"EOL
""EOL
"check(A - B)"EOL
""EOL
"# Note: It turns out that (G . grad)F actually means (grad F) . G"EOL
""EOL
"\"7. div(fF) = f div F + grad f . F\""EOL
""EOL
"A = div(f() F)"EOL
""EOL
"B = f() div(F) + dot(grad(f()),F)"EOL
""EOL
"check(A - B)"EOL
""EOL
"\"8. div(F x G) = G . curl F - F . curl G\""EOL
""EOL
"A = div(cross(F,G))"EOL
""EOL
"B = dot(G,curl(F)) - dot(F,curl(G))"EOL
""EOL
"check(A - B)"EOL
""EOL
"\"9. curl(fF) = f curl F + grad f x F\""EOL
""EOL
"A = curl(f() F)"EOL
""EOL
"B = f() curl(F) + cross(grad(f()),F)"EOL
""EOL
"check(A - B)"EOL
""EOL
"\"10. curl(F x G) = F div G - G div F + (G . grad)F - (F . grad)G\""EOL
""EOL
"A = curl(cross(F,G))"EOL
""EOL
"B = F div(G) - G div(F) + dot(grad(F),G) - dot(grad(G),F)"EOL
""EOL
"check(A - B)"EOL
""EOL
"# If we get here then everything worked."EOL
""EOL
"\"OK\""EOL,

// 3. RotationMatrix

"# This script demonstrates that the rotation matrix is orthogonal."EOL
"# We also demonstrate that the rotation matrix leaves the length of a vector"EOL
"# unchanged."EOL
"#"EOL
"# For an orthogonal matrix, inverse and transpose are equivalent."EOL
"#"EOL
"#        -1    T"EOL
"#	R   = R"EOL
"#"EOL
"# First we define the rotation matrix R then we convert sine and cosine"EOL
"# functions to their exponential forms."EOL
"#"EOL
"# The exponential forms lead to the required trigonometric simplifications."EOL
""EOL
"# These are the components of the rotation matrix."EOL
""EOL
"R11 = cos(phi2) cos(phi1) - cos(theta) sin(phi1) sin(phi2)"EOL
"R12 = -cos(phi2) sin(phi1) - cos(theta) cos(phi1) sin(phi2)"EOL
"R13 = sin(phi2) sin(theta)"EOL
""EOL
"R21 = sin(phi2) cos(phi1) + cos(theta) sin(phi1) cos(phi2)"EOL
"R22 = -sin(phi2) sin(phi1) + cos(theta) cos(phi1) cos(phi2)"EOL
"R23 = -cos(phi2) sin(theta)"EOL
""EOL
"R31 = sin(theta) sin(phi1)"EOL
"R32 = sin(theta) cos(phi1)"EOL
"R33 = cos(theta)"EOL
""EOL
"# R is the rotation matrix."EOL
""EOL
"R = ((R11,R12,R13),(R21,R22,R23),(R31,R32,R33))"EOL
""EOL
"# Print R"EOL
""EOL
"R"EOL
""EOL
"# Convert R to exponential form."EOL
""EOL
"expomode = 1"EOL
""EOL
"R = eval(R)"EOL
""EOL
"# Subtract transpose from inverse. The difference should be zero."EOL
""EOL
"check(inv(R) - transpose(R,1,2))"EOL
""EOL
"# Show that the rotation matrix leaves the length of a vector unchanged."EOL
"# The length difference between U and RU should be zero."EOL
""EOL
"U = (U1,U2,U3)"EOL
""EOL
"check(U^2 - dot(R,U)^2)"EOL
""EOL
"\"OK\""EOL,

// 4. QuantumHarmonicOscillator

"# \"Harmonic oscillator\" is the generic term for a system that has potential"EOL
"# energy V proportional to x squared."EOL
"#"EOL
"# For total energy E, kinetic energy K and potential energy V we have"EOL
"#"EOL
"#      E = K + V"EOL
"#"EOL
"# For QHO the equivalent Schroedinger equation is:"EOL
"#"EOL
"#      (2n + 1) psi = -d^2  psi / dx^2  + x^2  psi"EOL
"#"EOL
"# This differential equation can only be solved for integer values of n."EOL
"# The fact that n must be an integer is in agreement with physical reality:"EOL
"# the total energy of a QHO system is quantized."EOL
"#"EOL
"# The solution to the above equation is"EOL
"#"EOL
"#      psi(n,x) = exp(-1/2 x^2) Hn(x)"EOL
"#"EOL
"# Hn(x) is the nth Hermite polynomial in x."EOL
""EOL
"# Define the wave function."EOL
" "EOL
"psi(n) = exp(-1/2 x^2) hermite(x,n)"EOL
""EOL
"# We want to show that psi does indeed solve the Schroedinger equation."EOL
"# We start by defining the energy functions E, K and V."EOL
""EOL
"E(n) = (2 n + 1) psi(n)"EOL
""EOL
"K(n) = -d(d(psi(n),x),x)"EOL
""EOL
"V(n) = x^2 psi(n)"EOL
""EOL
"# Display a few wave functions."EOL
""EOL
"psi8 = condense(psi(8))"EOL
"psi9 = condense(psi(9))"EOL
"psi10 = condense(psi(10))"EOL
""EOL
"display(psi8)"EOL
"display(psi9)"EOL
"display(psi10)"EOL
""EOL
"\"Checking E = K + V for n = 0 to 10\""EOL
""EOL
"for(n, 0, 10, check(E(n) - K(n) - V(n)))"EOL
""EOL
"\"OK\""EOL,

// 5. HydrogenicWavefunctions

"# This script is a \"math manipulative\" for hydrogenic wavefunctions."EOL
"#"EOL
"# First we generate hydrogenic wavefunctions using all three quantum numbers."EOL
"#"EOL
"# Then we check that the wavefunctions solve the Schroedinger equation for the"EOL
"# hydrogen atom:"EOL
"#"EOL
"#     psi(n,L,m)/n^2 = del^2 psi(n,L,m) + 2 psi(n,L,m)/r"EOL
"#"EOL
"# We use upper case L to avoid confusion with the numeral 1."EOL
""EOL
"# Define the Laplacian operator for spherical coordinates (del-squared)."EOL
""EOL
"delsq(x) = 1/r^2 d(r^2 d(x,r),r) +"EOL
"  1/(r^2 sin(theta)) d(sin(theta) d(x,theta),theta) +"EOL
"  1/(r sin(theta))^2 d(d(x,phi),phi)"EOL
""EOL
"# psi is the product of radial wavefunction R and spherical harmonic Y"EOL
""EOL
"R(n,L) = r^L exp(-r/n) laguerre(2r/n,n-L-1,2L+1)"EOL
""EOL
"Y(L,m) = legendre(cos(theta),L,abs(m)) exp(i m phi)"EOL
""EOL
"psi(n,L,m) = R(n,L) Y(L,m)"EOL
""EOL
"# Define the energy functions E, K and V (from the Schroedinger equation)."EOL
""EOL
"E(n,L,m) = psi(n,L,m) / n^2"EOL
""EOL
"K(n,L,m) = delsq(psi(n,L,m))"EOL
""EOL
"V(n,L,m) = 2 psi(n,L,m) / r"EOL
""EOL
"\"Checking E = K + V for n = 1..5, L = 0..n-1, m = -L..L\""EOL
""EOL
"F(n,L,m) = simplify(E(n,L,m) - K(n,L,m) - V(n,L,m))"EOL
""EOL
"for(n,1,5,"EOL
"  for(L,0,n-1,"EOL
"    for(m,-L,L,"EOL
"      check(F(n,L,m))"EOL
")))"EOL
""EOL
"# If we get here then everything worked, print OK."EOL
""EOL
"\"OK\""EOL
""EOL
"# Print a few wavefunctions."EOL
""EOL
"psi320 = condense(psi(3,2,0))"EOL
"psi321 = condense(psi(3,2,1))"EOL
"psi322 = condense(psi(3,2,2))"EOL
""EOL
"display(psi320)"EOL
"display(psi321)"EOL
"display(psi322)"EOL,

// 6. StaticSphericalMetric

"# This script calculates the Einstein tensor for a static spherically symmetric"EOL
"# metric."EOL
"#"EOL
"# Cf. \"A first course in general relativity,\" Bernard F. Schutz, p. 255."EOL
"#"EOL
"# The book tells us exactly what the Einstein tensor components should be."EOL
"# If we get the right answer then we can be reasonably sure that the script is"EOL
"# correct. Once that is known then we can use the functions defined here in"EOL
"# other scripts."EOL
"#"EOL
"# This is the line element for the metric (Equation 10.7)"EOL
"#"EOL
"#   2     2 Phi   2    2 Lambda   2    2        2"EOL
"# ds  = -e      dt  + e         dr  + r  d Omega"EOL
"#"EOL
"# where"EOL
"#"EOL
"#  2        2    2         2      2            2"EOL
"# r  d Omega  = r  (d theta  + sin  theta d phi )"EOL
"#"EOL
"# Note: Phi and Lambda are both functions of r."EOL
""EOL
"# Given the line element we can write the metric tensor by inspection:"EOL
""EOL
"gdd = ((-exp(2 Phi(r)),                0,   0,                0),"EOL
"       (             0, exp(2 Lambda(r)),   0,                0),"EOL
"       (             0,                0, r^2,                0),"EOL
"       (             0,                0,   0, r^2 sin(theta)^2))"EOL
""EOL
"# Note: \"dd\" stands for two \"down\" indices, \"uu\" stands for two \"up\" indices."EOL
""EOL
"# X is our coordinate system. We need this for computing gradients."EOL
""EOL
"X = (t,r,theta,phi)"EOL
""EOL
"# Step 1: Calculate guu."EOL
""EOL
"guu = inv(gdd)"EOL
""EOL
"# Step 2: Calculate the connection coefficients. Cf. Gravitation, p. 210."EOL
"#"EOL
"# Gamma    = 1/2 (g     + g     - g    )"EOL
"#      abc         ab,c    ac,b    bc,a"EOL
"#"EOL
"# Note: The comma means gradient which increases the rank of gdd by 1."EOL
""EOL
"gddd = d(gdd,X)"EOL
""EOL
"# Note: We transpose indices so they match up with Gamma, i.e., we put them in"EOL
"# alphabetical order."EOL
""EOL
"GAMDDD = 1/2 (gddd +                # indices are already in correct order"EOL
"transpose(gddd,2,3) -               # transpose c and b"EOL
"transpose(transpose(gddd,2,3),1,2)) # transpose c and a, then b and a"EOL
""EOL
"# Raise first index."EOL
"#"EOL
"#      a      au"EOL
"# Gamma    = g   Gamma"EOL
"#       bc            ubc"EOL
"#"EOL
"# Note: Sum over index u means contraction."EOL
""EOL
"GAMUDD = contract(outer(guu,GAMDDD),2,3)"EOL
""EOL
"# Step 3. Calculate the Riemann tensor. Cf. Gravitation, p. 219."EOL
"#"EOL
"# a is alpha"EOL
"# b is beta"EOL
"# c is gamma"EOL
"# d is delta"EOL
"# u is mu"EOL
"#"EOL
"#  a           a            a            a        u          a        u"EOL
"# R     = Gamma      - Gamma      + Gamma    Gamma    - Gamma    Gamma"EOL
"#   bcd         bd,c         bc,d         uc       bd         ud       bc"EOL
"#"EOL
"# Do the gradient once and save in a temporary variable."EOL
""EOL
"tmp1 = d(GAMUDD,X)"EOL
""EOL
"# The Gamma Gamma product is a rank 6 tensor with dim 4 per rank."EOL
"# That works out to 4 to the 6th or 4,096 elements."EOL
"# Of course, we'll do the outer product and contract over u just once and save"EOL
"# the result in a second temporary variable."EOL
""EOL
"tmp2 = contract(outer(GAMUDD,GAMUDD),2,4)"EOL
""EOL
"# Now put it all together. Do the transpositions so the indices get matched up"EOL
"# with R on the left, i.e., put them in alphabetical order."EOL
""EOL
"RUDDD = transpose(tmp1,3,4) -             # transpose d and c"EOL
"  tmp1 +                                  # already in correct order"EOL
"  transpose(tmp2,2,3) -                   # transpose c and b"EOL
"  transpose(transpose(tmp2,2,3),3,4)      # transpose d and b, then d and c"EOL
""EOL
"# Step 4: Calculate the Ricci tensor. Cf. Gravitation, p. 343."EOL
"#"EOL
"#        a"EOL
"# R   = R"EOL
"#  uv     uav"EOL
"#"EOL
"# Contract over \"a\" (1st and 3rd indices)."EOL
""EOL
"RDD = contract(RUDDD,1,3)"EOL
""EOL
"# Step 5: Calculate the Ricci scalar. Cf. Gravitation, p. 343."EOL
"#"EOL
"#      uv"EOL
"# R = g   R"EOL
"#          vu  ...the book has uv, does it give the same result?"EOL
"#              Yes because the metric tensor is symmetric so it's ok to"EOL
"#              transpose."EOL
"#              I prefer vu because it looks like raising an index."EOL
""EOL
"R = contract(contract(outer(guu,RDD),2,3),1,2)"EOL
""EOL
"# Step 6: Finally, calculate the Einstein tensor. Cf. Gravitation, p. 343."EOL
"#"EOL
"# G   = R   - 1/2 g   R"EOL
"#  uv    uv        uv"EOL
""EOL
"GDD = RDD - 1/2 gdd R"EOL
""EOL
"# Next we compare this result with Schutz' book. Schutz p. 255 gives the"EOL
"# following Einstein tensor components (all other components are zero):"EOL
"#"EOL
"#        1                d"EOL
"# G   = ----  exp(2 Phi) ---- [r (1 - exp(-2 Lambda))]"EOL
"#  tt     2               dr"EOL
"#        r"EOL
"#"EOL
"#          1                                         2"EOL
"# G   = - ---- exp(2 Lambda) (1 - exp(-2 Lambda)) + --- Phi'"EOL
"#  rr       2                                        r"EOL
"#          r"EOL
"#"EOL
"#                 2                               2"EOL
"# G            = r  exp(-2 Lambda) [Phi'' + (Phi')  + Phi'/r"EOL
"#  theta theta"EOL
"#"EOL
"#                                                   - Phi' Lambda' - Lamda'/r]"EOL
"#"EOL
"#               2"EOL
"# G        = sin  theta G"EOL
"#  phi phi               theta theta"EOL
""EOL
"Gtt = 1/r^2 exp(2 Phi(r)) d(r (1 - exp(-2 Lambda(r))),r)"EOL
""EOL
"Grr = -1/r^2 exp(2 Lambda(r)) (1 - exp(-2 Lambda(r))) + 2/r d(Phi(r),r)"EOL
""EOL
"Gthetatheta = r^2 exp(-2 Lambda(r)) ("EOL
"  d(d(Phi(r),r),r) +"EOL
"  d(Phi(r),r)^2 +"EOL
"  d(Phi(r),r) / r -"EOL
"  d(Phi(r),r) d(Lambda(r),r) -"EOL
"  d(Lambda(r),r) / r)"EOL
""EOL
"Gphiphi = sin(theta)^2 Gthetatheta"EOL
""EOL
"# Put together the expected tensor:"EOL
""EOL
"expect = ((Gtt,   0,           0,       0),"EOL
"          (  0, Grr,           0,       0),"EOL
"          (  0,   0, Gthetatheta,       0),"EOL
"          (  0,   0,           0, Gphiphi))"EOL
""EOL
"# Check that GDD is correct."EOL
""EOL
"check(GDD - expect)"EOL
""EOL
"# Display the non-zero components of GDD."EOL
""EOL
"display(Gtt)"EOL
"display(Grr)"EOL
"display(Gthetatheta)"EOL
"display(Gphiphi)"EOL
""EOL
"\"OK\""EOL,

// 7. FreeParticleDiracEquation

"# This script demonstrates the free particle Dirac equation and a few of its"EOL
"# solutions."EOL
"#"EOL
"# \"Free particle\" means that there is no force pushing the particle around. In"EOL
"# other words, the potential energy field V is zero everywhere. What remains is"EOL
"# just the total energy T and kinetic energy K."EOL
"#"EOL
"#     T = K"EOL
"#"EOL
"# The equivalent Dirac equation is"EOL
"#"EOL
"#     m Psi = i delslash Psi"EOL
"#"EOL
"# where m is the particle's rest mass, Psi is a 4-vector and delslash is a"EOL
"# differential operator involving gamma matrices."EOL
"#"EOL
"# In terms of physical units, the above equation has m in the wrong place. The"EOL
"# mass m should be a divisor in the kinetic energy term. However, the equation"EOL
"# is more cool-looking as shown and anyway that's how Feynman wrote it. We just"EOL
"# have to remember to divide the above equation by m to get physical units of"EOL
"# energy."EOL
""EOL
"# Verify a few solutions (Psi) to the above free-particle Dirac equation."EOL
""EOL
"# Define the spacetime metric."EOL
""EOL
"metric = ((-1,0,0,0),(0,-1,0,0),(0,0,-1,0),(0,0,0,1))"EOL
""EOL
"# Define the gamma matrices."EOL
""EOL
"gammax = (( 0, 0, 0, 1),( 0, 0, 1, 0),( 0,-1, 0, 0),(-1, 0, 0, 0))"EOL
"gammay = (( 0, 0, 0,-i),( 0, 0, i, 0),( 0, i, 0, 0),(-i, 0, 0, 0))"EOL
"gammaz = (( 0, 0, 1, 0),( 0, 0, 0,-1),(-1, 0, 0, 0),( 0, 1, 0, 0))"EOL
"gammat = (( 1, 0, 0, 0),( 0, 1, 0, 0),( 0, 0,-1, 0),( 0, 0, 0,-1))"EOL
""EOL
"# Define the delslash operator."EOL
""EOL
"delslash(f) ="EOL
"  dot(gammax,d(f,x))+"EOL
"  dot(gammay,d(f,y))+"EOL
"  dot(gammaz,d(f,z))+"EOL
"  dot(gammat,d(f,t))"EOL
""EOL
"# Define energy E, momentum p and coordinate X."EOL
""EOL
"E = sqrt(px^2 + py^2 + pz^2 + m^2)"EOL
"p = (px,py,pz,E)"EOL
"X = (x,y,z,t)"EOL
""EOL
"# Verify that p.p = m^2"EOL
""EOL
"check(dot(p,metric,p) - m^2) # continue if the difference is zero, else stop"EOL
""EOL
"# Define the solutions."EOL
""EOL
"PsiA = (E+m,0,pz,px+i py) exp(-i dot(p,metric,X))"EOL
"PsiB = (0,E+m,px-i py,-pz) exp(-i dot(p,metric,X))"EOL
"PsiC = (pz,px+i py,E+m,0) exp(i dot(p,metric,X))"EOL
"PsiD = (px-i py,-pz,0,E+m) exp(i dot(p,metric,X))"EOL
""EOL
"# Verify the solutions."EOL
""EOL
"check(m PsiA - i delslash(PsiA))"EOL
"check(m PsiB - i delslash(PsiB))"EOL
"check(m PsiC - i delslash(PsiC))"EOL
"check(m PsiD - i delslash(PsiD))"EOL
""EOL
"# Try a linear combination of all solutions."EOL
""EOL
"Psi = A PsiA + B PsiB + C PsiC + D PsiD"EOL
""EOL
"check(m Psi - i delslash(Psi))"EOL
""EOL
"# For PsiA and PsiB it turns out that"EOL
"#"EOL
"#     i delslash Psi = pslash Psi"EOL
"#"EOL
"# So another form of the free particle Dirac equation is"EOL
"#"EOL
"#     m Psi = pslash Psi"EOL
"#"EOL
"# Verify solutions to the above equation."EOL
""EOL
"# Define the gamma tensor. See Gamma Matrix Algebra for details."EOL
""EOL
"gamma ="EOL
"  outer(gammax,(1,0,0,0)) +"EOL
"  outer(gammay,(0,1,0,0)) +"EOL
"  outer(gammaz,(0,0,1,0)) +"EOL
"  outer(gammat,(0,0,0,1))"EOL
""EOL
"# Dot gamma with p to get pslash."EOL
""EOL
"pslash = dot(gamma,metric,p)"EOL
""EOL
"# Verify the solutions again."EOL
""EOL
"check(m PsiA - dot(pslash,PsiA))"EOL
"check(m PsiB - dot(pslash,PsiB))"EOL
"check(m PsiC + dot(pslash,PsiC))"EOL
"check(m PsiD + dot(pslash,PsiD))"EOL
""EOL
"# Display pslash on the computer screen."EOL
""EOL
"pslash = subst(quote(E),E,pslash) # subst letter E for the energy expression"EOL
""EOL
"display(pslash)"EOL
""EOL
"# If we get here then everything worked, print OK."EOL
""EOL
"\"OK\""EOL
};
