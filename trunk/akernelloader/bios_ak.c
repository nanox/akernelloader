/*  Akernelloader TEAM
    akaloaderadmin@gmail.com

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
   USA. 
 */

#include <bios_ak.h>
#include <types_ak.h>

 /* if error occurs, then return the error number. 
    Otherwise, return 0.  */
int bios_disk_ak(int rw, int drive, uint64 sector, int nsec, int segment)
{
  int error;
  struct drv_parameters parameters;

                 get_parameters (driver_bios , &parameters);


  
      dap.length = sizeof (dap);
      dap.block = sector;
      dap.blocks = nsec;
      dap.reserved = 0;
      dap.buffer = segment << 16;
      
      error = bios_rw_lba_ak(rw, drive, &dap);

      if(error){

      int cylinder_offset, head_offset, sector_offset;
      int head;
      
       unsigned sector_chs = (unsigned) sector;

      /* sector_offset is counted from one, while HEAD_OFFSET and
	 cylinder_offset are counted from zero.  */
      sector_offset = sector_chs % parameters.sectors + 1;
      head = sector_chs / parameters.sectors;
      head_offset = head % parameters.heads;
      cylinder_offset = head / parameters.heads;
      

      error = bios_rw_chs_ak ( rw, drive,
			       cylinder_offset, head_offset, sector_offset,
			       nsec, segment);
    }

  return error;
}



int get_parameters (int drive, struct drv_parameters *parameters)
{
  int err;


      err = get_drive_param_ak (drive,
				   &parameters->cylinders,
				   &parameters->heads,
				   &parameters->sectors);
      if (err)
	return err;


      parameters->total_sectors = (parameters->cylinders
				 * parameters->heads
				 * parameters->sectors);
      parameters->sector_size = SECTOR_SIZE;
    

  return 0;
}



