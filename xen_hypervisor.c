#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VMS 10

typedef struct {
    int id;
    char name[20];
    int cpu_allocation;
    int memory_allocation;
    int is_running;
} VM;

typedef struct {
    VM *vms[MAX_VMS];
    int vm_count;
} Hypervisor;

// Initialize the hypervisor
void initialize_hypervisor(Hypervisor *hypervisor) {
    hypervisor->vm_count = 0;
    for (int i = 0; i < MAX_VMS; i++) {
        hypervisor->vms[i] = NULL;
    }
    printf("Hypervisor initialized.\n");
}

// Create a new VM
VM* create_vm(int id, const char *name, int cpu, int memory) {
    VM *vm = (VM *)malloc(sizeof(VM));
    if (vm == NULL) {
        perror("Failed to allocate memory for VM");
        exit(EXIT_FAILURE);
    }
    vm->id = id;
    strncpy(vm->name, name, sizeof(vm->name) - 1);
    vm->name[sizeof(vm->name) - 1] = '\0';
    vm->cpu_allocation = cpu;
    vm->memory_allocation = memory;
    vm->is_running = 0;
    return vm;
}

// Add a VM to the hypervisor
void add_vm(Hypervisor *hypervisor, VM *vm) {
    if (hypervisor->vm_count < MAX_VMS) {
        hypervisor->vms[hypervisor->vm_count++] = vm;
        printf("VM %s added.\n", vm->name);
    } else {
        printf("Cannot add VM. Maximum limit reached.\n");
    }
}

// Start a VM
void start_vm(VM *vm) {
    if (!vm->is_running) {
        vm->is_running = 1;
        printf("VM %s started.\n", vm->name);
    } else {
        printf("VM %s is already running.\n", vm->name);
    }
}

// Stop a VM
void stop_vm(VM *vm) {
    if (vm->is_running) {
        vm->is_running = 0;
        printf("VM %s stopped.\n", vm->name);
    } else {
        printf("VM %s is not running.\n", vm->name);
    }
}

// Communication between VMs (simplified)
void send_message(VM *sender, VM *receiver, const char *message) {
    if (sender->is_running && receiver->is_running) {
        printf("VM %s sent message to VM %s: %s\n", sender->name, receiver->name, message);
    } else {
        printf("Both VMs must be running to send messages.\n");
    }
}

// Cleanup and free memory
void cleanup_hypervisor(Hypervisor *hypervisor) {
    for (int i = 0; i < hypervisor->vm_count; i++) {
        free(hypervisor->vms[i]);
    }
    hypervisor->vm_count = 0;
    printf("Hypervisor cleaned up.\n");
}

int main() {
    Hypervisor hypervisor;
    
    initialize_hypervisor(&hypervisor);
    
    VM *vm1 = create_vm(1, "VM1", 2, 2048);
    VM *vm2 = create_vm(2, "VM2", 4, 4096);
    
    add_vm(&hypervisor, vm1);
    add_vm(&hypervisor, vm2);
    
    start_vm(vm1);
    start_vm(vm2);
    
    send_message(vm1, vm2, "Hello from VM1 to VM2!");
    
    stop_vm(vm1);
    stop_vm(vm2);
    
    cleanup_hypervisor(&hypervisor);
    
    return 0;
}
