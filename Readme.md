
# Obs Graph Tool 2022 Readme 

Brief instruction on the preparation of the Observation Graph Tool.

## Introduction about the tools

After the tool is [prepared and compiled properly](https://github.com/ObsGraphTool/Version2022#how-to-run-this-project-locally) , use the following command at the tool's path after making the project.
```
./ObsGraph
```
### User Interface
Then there shall be an interface as follows. 

![Welcome](https://user-images.githubusercontent.com/97455443/196976602-3f5280d9-2a1d-4c61-803b-dc23bc79d19f.png)
### File input- Example Contractor and Subcontract
Where you can indicate the number of the service you would like compose before substitution. Then carefully input the path of the example files you would like to choose.

![enter1](https://user-images.githubusercontent.com/97455443/196976588-88087930-dd85-4d4e-a3fa-1ef027cf53de.png)

![enter2](https://user-images.githubusercontent.com/97455443/196976594-e6abc3b6-1800-424c-bc08-8014e9aeb147.png)

Here I put the example in the folder /ex, it's an example about **a contractor** (File name as 1.net, 2, 3 respectively the Net file, Observables, and the Final Place)and **a subcontractor** (File name as 4.net, 5, 6 respectively the Net file, Observables, and the Final Place). The WorkFlow-net of the model is shown below. We can see after the input of the net files is composed, the subnets have been created successfully and the pieces of information about the subnets are displayed.

![WFnetCon](https://user-images.githubusercontent.com/97455443/196978825-3badcdfa-79dc-4a75-990b-98663726c32a.png)

### Composition Result Demostration
Then after the completion of the input, a result will show, indicating if the composition is deadlockfree or not.

![result](https://user-images.githubusercontent.com/97455443/196976597-4dc181c0-877a-48d1-80a3-498da7041cf7.png)

After the demostration of the result, you could choose whether to ssubstitutea service or not. 

![sub](https://user-images.githubusercontent.com/97455443/196976598-7e3bb770-2194-410d-982d-46e531f51a06.png)

### Service Substitution

Here I choose substitute service No.1, which is the service of the Contractor. I will substitute it with another service called alternative Contractor(File names A1, 2, and 3 respectively the Net file, Observables, and the Final Place). The workflow net of the Alternative Contractor can be seen as follows:

![alt](https://user-images.githubusercontent.com/97455443/196980999-932be8e7-493b-4594-a7c7-022f606b65e3.jpg)

Such alternative net will create a deadlock in the composition, the result will indicate that there is a deadlock state, as follows:

![subresult](https://user-images.githubusercontent.com/97455443/196976599-561076ac-5500-4bd1-b568-14b4b225db7a.png)

### ISAP Sample
The example used for ISAP are also in the example folder /ex, and it's named by the role of the service, such as supplier.net, vendor.net or client.net, two medium net respectively m1 and m2 and their properties (Observables and Final places) are also in the folder.

The WF-net of the ISAP example is shown in the below picture.

![ISAP example](https://user-images.githubusercontent.com/97455443/196983789-23e2d5c7-cbe4-4601-a76d-10f331296cf4.png)

## How to run this project locally

### Make sure you have git at your local machine
If you are using Linux you can use simply 
```
git version
```
to check whether you have installed git on your machine. If not, it's a good idea to make sure you're running the latest version. To do so, Navigate to your command prompt shell and run the following command to make sure everything is up-to-date:
```sudo apt-get update```
To install Git, run the following command: ```sudo apt-get install git-all```

###  Clone this project to local
In the terminal, switch to the folder you are comfortable with to put the project under.and in the target folder, use the Git command to clone the project.
 
```
git clone https://github.com/ObsGraphTool/Version2022.git
```


## Setup Instructions

To deploy this project, please download and unzip the file "ObsGraphToolS.zip" in the repository, **a Unix or Linux** environment is preferred to successfully compile the tool.

### 1. Compiler tools to be installed

In the terminal, Install the following tools. In case of using **yum**, please adapt likewise.

```bash
  sudo apt-get install bison
  sudo apt-get install flex
```

Those are the two compiler tools needed to determine some explanatory syntax in the code.

### 2. Delete all the .d files in the /obj folder

If it's a newly unzipped source code, this step can be skipped. 
If it's been run/compiled a few times, please clear the **./obj folder** to avoid errors.

### 3. Make Buddy Library

In terminal, go to the folder **./buddy22**, then **clean** first. 
```
make clean
```
After clean is successful, 
```
make
```
Then **Don't forget to
``` 
make install
```
If there is such code pops out it's installed successfully
```
cp -f src/libbdd.a ./lib/libbdd.a
chmod 644 ./lib/libbdd.a
cp -f src/bdd.h ./include/bdd.h
chmod 644 ./include/bdd.h
cp -f src/fdd.h ./include/fdd.h
chmod 644 ./include/fdd.h
cp -f src/bvec.h ./include/bvec.h
chmod 644 ./include/bvec.h
```

### 4. Make Parser Library
In the terminal, go to the folder ../parser and do the make clean + make again
```
make clean
~
make 
```

### 5. Make the ObsGraphTool

In the terminal, go to the ./ObsGraphTool folder (Father folder), clean first and then make all, then the tool is ready to be tested.
```
make clean
~
make all
```

If the following code appears when make clean or make , the **parser library** wasn't made properly.
```
collect2: error: ld returned 1 exit status
make: *** [Makefile:121: ObsGraph] Error 1
```
If the following code appears when make clean or make , the **/obj folder** wasn't cleared properly.
```
make: *** No rule to make target '/Library/Developer/CommandLineTools/usr/bin/../include/c++/v1/iostream', needed by 'obj/Simple_MDGraph.d'.  Stop.

```
## Related Paper

Examples and some algorithms used to design and test the tool can be found the following papers

[Symbolic abstraction and deadlock-freeness verification of inter-enterprise processes
](https://www.sciencedirect.com/science/article/pii/S0169023X11000140)

[A Bottom-Up Approach to Check the Correctness of Interorganisational Workflows](https://ieeexplore.ieee.org/document/7307728)

## Maintenance and Issues
If any problems or troubles are encounted during using the tool, please contact Professor Hanen OCHI (Hanen.ochi@efrei.fr) or Yanwu ZHU (Yanwu.zhu@efrei.net)
