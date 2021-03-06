#ifndef INIT_MATRIX_TASKS_H
#define INIT_MATRIX_TASKS_H

#include "legion.h"
#include "hodlr_matrix.h"

using namespace LegionRuntime::HighLevel;
using namespace LegionRuntime::Accessor;

void register_init_tasks();

class RandomMatrixTask : public TaskLauncher {
 public:
  struct TaskArgs {
    long  seed;
    Range columns;
  };
  
  RandomMatrixTask(TaskArgument arg,
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

class DenseMatrixTask : public TaskLauncher {
 public:
  template <int N>
    struct TaskArgs {
      int row;
      int rank;
      double diag;
      Node treeArray[N];
    };
  
  DenseMatrixTask(TaskArgument arg,
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

class CirculantMatrixTask : public TaskLauncher {
 public:
  struct TaskArgs {
    int col_beg;
    int row_beg;
    int rank;
  };
    
  CirculantMatrixTask(TaskArgument arg,
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
