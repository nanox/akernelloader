#include <string_ak.h>
#include <elf_ak.h>
#include <types_ak.h>

int IS_ELF(void* address){
     
      Elf32_Ehdr *elf;        //elf file header
      int ret = 1;

      elf = (Elf32_Ehdr *) address;  
      
     if ((elf->e_ident[0] !=0x7f ||    
        elf->e_ident[1] !='E' ||  
        elf->e_ident[2] !='L' ||  
        elf->e_ident[3] !='F') &&
         (elf->e_type != ET_EXEC ))
        ret = 0 ;  
        return ret ;
}

void load_elf(void* mem_addr,__u32  *entry){

    Elf32_Ehdr *elf;        //elf file header  
    Elf32_Phdr *elf_phdr;   /* program segment header */   
    int i ;

    elf = (Elf32_Ehdr *) mem_addr;

    unsigned long e_phoff = elf->e_phoff;
    void *e_entry = (void*)elf->e_entry;

    for (i = 0; i < elf->e_phnum; ++i)  
    {  
         
        elf_phdr = (Elf32_Phdr *) ( e_phoff +  mem_addr);  
          
        e_phoff += sizeof(*elf_phdr);  
  
        if (elf_phdr->p_type != PT_LOAD)  
            break;  
  
        memmove_akel((void *)(elf_phdr->p_vaddr), (void*)(elf_phdr->p_offset+mem_addr), elf_phdr->p_filesz);  
        memset_akel((char*)(elf_phdr->p_vaddr+elf_phdr->p_filesz), 0, elf_phdr->p_memsz - elf_phdr->p_filesz);
    }  
   
      *entry = e_entry ;
  
}


