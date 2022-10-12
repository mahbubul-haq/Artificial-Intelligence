import org.chocosolver.solver.Model;
import org.chocosolver.solver.Solver;
import org.chocosolver.solver.variables.IntVar;


public class Kendoku {
public static void main(String[] args) {


int i, j, k;


Model model = new Model("my first kendoku-3 problem");


IntVar[][] bd = model.intVarMatrix("bd", 6, 6, 1, 6);


IntVar[] r0 = model.intVarArray("r0", 6, 1, 6);
IntVar[] r1 = model.intVarArray("r1", 6, 1, 6);
IntVar[] r2 = model.intVarArray("r2", 6, 1, 6);
IntVar[] r3 = model.intVarArray("r3", 6, 1, 6);
IntVar[] r4 = model.intVarArray("r4", 6, 1, 6);
IntVar[] r5 = model.intVarArray("r5", 6, 1, 6);


IntVar[] c0 = model.intVarArray("c0", 6, 1, 6);
IntVar[] c1 = model.intVarArray("c1", 6, 1, 6);
IntVar[] c2 = model.intVarArray("c2", 6, 1, 6);
IntVar[] c3 = model.intVarArray("c3", 6, 1, 6);
IntVar[] c4 = model.intVarArray("c4", 6, 1, 6);
IntVar[] c5 = model.intVarArray("c5", 6, 1, 6);

    
for ( j = 0; j < 6; j++)
  model.arithm (bd[0][j], "=", r0[j]).post();
  
for ( j = 0; j < 6; j++)
  model.arithm (bd[1][j], "=", r1[j]).post();
    
for ( j = 0; j < 6; j++)
  model.arithm (bd[2][j], "=", r2[j]).post();

for ( j = 0; j < 6; j++)
  model.arithm (bd[3][j], "=", r3[j]).post();
  
for ( j = 0; j < 6; j++)
  model.arithm (bd[4][j], "=", r4[j]).post();
    
for ( j = 0; j < 6; j++)
  model.arithm (bd[5][j], "=", r5[j]).post();





for ( i = 0; i < 6; i++)
  model.arithm (bd[i][0], "=", c0[i]).post();

for ( i = 0; i < 6; i++)
  model.arithm (bd[i][1], "=", c1[i]).post();

for ( i = 0; i < 6; i++)
  model.arithm (bd[i][2], "=", c2[i]).post();

for ( i = 0; i < 6; i++)
  model.arithm (bd[i][3], "=", c3[i]).post();

for ( i = 0; i < 6; i++)
  model.arithm (bd[i][4], "=", c4[i]).post();

for ( i = 0; i < 6; i++)
  model.arithm (bd[i][5], "=", c5[i]).post();


model.allDifferent(r0).post();
model.allDifferent(r1).post();
model.allDifferent(r2).post();
model.allDifferent(r3).post();
model.allDifferent(r4).post();
model.allDifferent(r5).post();



model.allDifferent(c0).post();
model.allDifferent(c1).post();
model.allDifferent(c2).post();
model.allDifferent(c3).post();
model.allDifferent(c4).post();
model.allDifferent(c5).post();


String add = "+", eq = "=", sub = "-", div="/";

IntVar t1 = model.intVar("t1", 1, 36);
model.times(r0[0], r0[1], t1).post();
model.times(t1, r1[0], 10).post();

model.arithm(r4[5], add, r5[5], eq, 8).post();

IntVar t2 = model.intVar("t2", 1, 36);
model.times(r0[2], r0[3], t2).post();
model.times(t2, r1[2], 48).post();

IntVar t3 = model.intVar("t3", 1, 12);

model.arithm(r0[4], add, r1[3], eq, t3).post();
IntVar t4 = model.intVar("t4", 1, 18);
model.arithm(r1[4], add, t3, eq, t4).post();
model.arithm(r2[4], add, t4, eq, 14).post();

IntVar t5 = model.intVar("t5", 1, 36);
model.times(r2[0], r3[0], t5).post();
model.times(t5, r4[0], 72).post();

model.times(r5[2], r5[3], 30).post();


IntVar t6 = model.intVar("t6", 1, 12);

model.arithm(r2[3], add, r3[3], eq, t6).post();
model.arithm(r3[4], add, t6, eq, 10).post();

IntVar t7 = model.intVar("t7", -5, 5);

model.arithm(r0[5], sub , r1[5], eq, t7).post();
model.times(t7, t7, 4).post();

IntVar t8 = model.intVar("t8", -5, 5);

model.arithm(r1[1], sub , r2[1], eq, t8).post();
model.times(t8, t8, 1).post();

IntVar t9 = model.intVar("t9", -5, 5);

model.arithm(r2[5], sub , r3[5], eq, t9).post();
model.times(t9, t9, 9).post();

IntVar t10 = model.intVar("t10", -5, 5);

model.arithm(r4[2], sub , r4[3], eq, t10).post();
model.times(t10, t10, 16).post();


     Solver solver = model.getSolver();

    solver.showStatistics();
    solver.showSolutions();
    solver.findSolution();


// 5. Print the solution

for ( i = 0; i < 6; i++)
    {
for ( j = 0; j < 6; j++)
     { 
  
        System.out.print(" "); 
        /* get the value for the board position [i][j] for the solved board */
        k = bd [i][j].getValue();
        System.out.print(k );
     }
     System.out.println();
    }


}

}


