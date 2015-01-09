#ifndef INIT_MATRIX_TASKS_H
#define INIT_MATRIX_TASKS_H

#include "legion.h"
#include "Htree.h"

using namespace LegionRuntime::HighLevel;


void register_init_tasks();


class InitRHSTask : public TaskLauncher {
 public:
  struct TaskArgs {
    int rand_seed;
    int ncol;
    //char filename[25];
  };
  
  InitRHSTask(TaskArgument arg,
	      Predicate pred = Predicate::TRUE_PRED,
	      MapperID id = 0,
	      MappingTagID tag = 0);
  
  static int TASKID;
  static void register_tasks(void);

 public:
  static void cpu_task(const Task *task,
		       const std::vector<PhysicalRegion> &regions,
		       Context ctx, HighLevelRuntime *runtime);
};

class InitCirculantKmatTask : public TaskLauncher {
 public:
  template <int N>
    struct TaskArgs {
      //int treeSize;
      int row_beg_global;
      int rank;
      //int LD; // leading dimension
      double diag;
      FSTreeNode treeArray[N];
    };
  
  InitCirculantKmatTask(TaskArgument arg,
			Predicate pred = Predicate::TRUE_PRED,
			MapperID id = 0,
			MappingTagID tag = 0);
  
  static int TASKID;
  static void register_tasks(void);

 public:
  static void cpu_task(const Task *task,
		       const std::vector<PhysicalRegion> &regions,
		       Context ctx, HighLevelRuntime *runtime);
};

class InitCirculantMatrixTask : public TaskLauncher {
 public:
  struct TaskArgs {
    int col_beg;
    int row_beg;
    int rank;
  };
    
  InitCirculantMatrixTask(TaskArgument arg,
			  Predicate pred = Predicate::TRUE_PRED,
			  MapperID id = 0,
			  MappingTagID tag = 0);
  
  static int TASKID;
  static void register_tasks(void);

 public:
  static void cpu_task(const Task *task,
		       const std::vector<PhysicalRegion> &regions,
		       Context ctx, HighLevelRuntime *runtime);
};





#endif // INIT_MATRIX_TASKS_H