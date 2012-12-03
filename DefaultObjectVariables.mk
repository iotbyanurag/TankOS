
KERNEL := AntonAvrLib/$(BUILD_DIRNAME)/kernel
SHARED := Tank-Shared/$(BUILD_DIRNAME)
UNITY := Unity/$(BUILD_DIRNAME)/unity.o

# This is convenience to configure objects of testrunner mains
# Usage:
# $(eval $(call set_test_objects,test_file, some.o additional.o test.o objects.o))
define set_test_objects
	objects_$(project)_testrunners/$1.testrunner := $(UNITY) $(BUILDDIR)/$1.test.o $2
endef