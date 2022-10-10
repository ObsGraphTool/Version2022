
# Obs Graph Tool 2022 Readme 

A brief instruction of the preparation of Observation Graph Tool .


## Deployment

To deploy this project, please download and unzip the file "ObsGraphToolS.zip" in the repository, **a Unix or Linux** environment is preferred to successfully compile the tool.

### 1. Compiler tools to be installed

In terminal, Install the following tools. In case of using **yum**, please adapt likewise.

```bash
  sudo apt-get install bison
  sudo apt-get install flex
```

Those are the two compiler tools needed to determine some explanatory syntax in the code.

### 2. Delete all the .d file in the /obj folder

If it's a newly unzipped source code, this step can be skipped. 
If it's been run/compiled a few times, please clear the **./obj folder** to avoid errors.

### 3. Make Buddy Library

In terminal, go to the folder **./buddy22**, then **clean** first. 
```
make clean
```
After clean is successfully, 
```
make
```
Then **Don't forget to
``` 
make install
```
If there such code pops out it's installed successfully
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
In terminal, go to the folder ../parser and do the make clean + make again
```
make clean
~
make 
```

### 5. Make the ObsGraphTool

In terminal, go to the ./ObsGraphTool folder (Father folder) , clean first and then make all, then the tool is ready be tested.
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

Examples used to test the tool can be found in this paper

[Symbolic abstraction and deadlock-freeness verification of inter-enterprise processes
](https://www.sciencedirect.com/science/article/pii/S0169023X11000140)


## Test examples/usages

```
./ObsGraph -S ModularModels/Contractor+SubcontractorAlt/Prod/Contractor.net -OModularModels/Contractor+SubcontractorAlt/Prod/Obs_Contractor ModularModels/Contractor+SubcontractorAlt/Prod/Fplace_Contractor ModularModels/Contractor+SubcontractorAlt/Prod/inet_ContSubCont.net -OModularModels/Contractor+SubcontractorAlt/Prod/Obs_inet_ContSubCont ModularModels/Contractor+SubcontractorAlt/Prod/Fplace_inet_ContSubCont ModularModels/Contractor+SubcontractorAlt/Prod/Subcontractor.net -OModularModels/Contractor+SubcontractorAlt/Prod/Obs_Subcontractor ModularModels/Contractor+SubcontractorAlt/Prod/Fplace_Subcontractor 1

./ObsGraph -S ModularModels/Contractor+Subcontractor/Prod/Contractor.net -OModularModels/Contractor+Subcontractor/Prod/Obs_Contractor ModularModels/Contractor+Subcontractor/Prod/Fplace_Contractor ModularModels/Contractor+Subcontractor/Prod/inet_ContSubCont.net -OModularModels/Contractor+Subcontractor/Prod/Obs_inet_ContSubCont ModularModels/Contractor+Subcontractor/Prod/Fplace_inet_ContSubCont ModularModels/Contractor+Subcontractor/Prod/Subcontractor.net -OModularModels/Contractor+Subcontractor/Prod/Obs_Subcontractor ModularModels/Contractor+Subcontractor/Prod/Fplace_Subcontractor 1

./ObsGraph -R ModularModels/Contractor+SubcontractorAlt/Prod/Contractor.net -OModularModels/Contractor+SubcontractorAlt/Prod/Obs_Contractor ModularModels/Contractor+SubcontractorAlt/Prod/Fplace_Contractor ModularModels/Contractor+SubcontractorAlt/Prod/inet_ContSubCont.net -OModularModels/Contractor+SubcontractorAlt/Prod/Obs_inet_ContSubCont ModularModels/Contractor+SubcontractorAlt/Prod/Fplace_inet_ContSubCont ModularModels/Contractor+SubcontractorAlt/Prod/Subcontractor.net -OModularModels/Contractor+SubcontractorAlt/Prod/Obs_Subcontractor ModularModels/Contractor+SubcontractorAlt/Prod/Fplace_Subcontractor 1

./ObsGraph -S ModularModels/Contractor+SubcontractorAlt/Prod/Contractor.net ALL ModularModels/Contractor+SubcontractorAlt/Prod/Fplace_Contractor ModularModels/Contractor+SubcontractorAlt/Prod/inet_ContSubCont.net -OModularModels/Contractor+SubcontractorAlt/Prod/Obs_inet_ContSubCont ModularModels/Contractor+SubcontractorAlt/Prod/Fplace_inet_ContSubCont ModularModels/Contractor+SubcontractorAlt/Prod/Subcontractor.net -OModularModels/Contractor+SubcontractorAlt/Prod/Obs_Subcontractor ModularModels/Contractor+SubcontractorAlt/Prod/Fplace_Subcontractor 1

./ObsGraph -S ModularModels/Contractor+SubcontractorAlt/Prod/Contractor.net ALL ModularModels/Contractor+SubcontractorAlt/Prod/Fplace_Contractor ModularModels/Contractor+SubcontractorAlt/Prod/inet_ContSubCont.net ALL ModularModels/Contractor+SubcontractorAlt/Prod/Fplace_inet_ContSubCont ModularModels/Contractor+SubcontractorAlt/Prod/Subcontractor.net -OModularModels/Contractor+SubcontractorAlt/Prod/Obs_Subcontractor ModularModels/Contractor+SubcontractorAlt/Prod/Fplace_Subcontractor 1

./ObsGraph ModularModels/Contractor+SubcontractorAlt/Prod/Contractor.net ALL ModularModels/Contractor+SubcontractorAlt/Prod/Fplace_Contractor 1

./ObsGraph -S ModularModels/Contractor+Subcontractor/Prod/AsynchCompoContractorSubContractor.net -OModularModels/Contractor+Subcontractor/Prod/Obs_AsynchCompoContractorSubContractor.txt ModularModels/Contractor+Subcontractor/Prod/Fplace_AsynchCompoContractorSubContractor.txt 1

./ObsGraph -S -OModularModels/Contractor+Subcontractor/Prod/AsynchCompoContractorSubContractor.net ModularModels/Contractor+Subcontractor/Prod/Obs_AsynchCompoContractorSubContractor.txt ModularModels/Contractor+Subcontractor/Prod/Fplace_AsynchCompoContractorSubContractor.txt 1

./ObsGraph ModularModels/Contractor+SubcontractorAlt/Prod/Contractor.net EMPTY ModularModels/Contractor+SubcontractorAlt/Prod/Fplace_Contractor 1

./ObsGraph ModularModels/Contractor+SubcontractorAlt/Prod/Contractor.net -o«order1 spec1 ship1 cost1» ModularModels/Contractor+SubcontractorAlt/Prod/Fplace_Contractor 1

Exemple dead et relax sound
./ObsGraph -R Test_relaxS_Dead -OObs_Test_not_relaxS_MAJ_mf_tf Fplace_Test_not_relaxS_MAJ_mf_tf 1
```


## Support

For support, email Yanwu.zhu@efrei.net, or post an issue at this repository.

