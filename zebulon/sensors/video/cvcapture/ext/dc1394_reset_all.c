/*
 * resets the bus using libdc1394
 *
 * Written by David Moore
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <stdio.h>
#include <stdint.h>
#include <dc1394/dc1394.h>
#include <stdlib.h>
#include <inttypes.h>
#include <limits.h>


int main(int argc, char *argv[])
{
    dc1394_t * d;
    dc1394camera_list_t * list;
    dc1394camera_t *camera;
    dc1394error_t err;

    d = dc1394_new ();

    if (!d)
        return 1;
    int cam=0;
    int num=-1;
    int chan;
    unsigned int val;
    do
    {
        err=dc1394_camera_enumerate (d, &list);
        DC1394_ERR_RTN(err,"Failed to enumerate cameras");

        if (list->num == 0) {
            dc1394_log_error("No cameras found");
            return 1;
        }
        else
        {
            num=list->num;
        }
        printf("Camera %d\n",cam);
        camera = dc1394_camera_new (d, list->ids[cam].guid);
        if (!camera) {
            dc1394_log_error("Failed to initialize camera with guid %llx", list->ids[cam].guid);
            return 1;
        }
        dc1394_camera_free_list (list);

        printf("Using camera with GUID %"PRIx64"\n", camera->guid);

        printf ("Reseting bus...\n");
        if (dc1394_reset_bus (camera) != DC1394_SUCCESS)
            printf ("Warning: reset reported error\n");
            
        dc1394_capture_stop(camera);
        dc1394_iso_release_all(camera);
        if ( dc1394_video_get_bandwidth_usage(camera,&val) == DC1394_SUCCESS
            && dc1394_iso_release_bandwidth(camera,val) == DC1394_SUCCESS)
        {
            printf("Freed %d Bandwidth\n",val);
        }
        
        if ( dc1394_video_get_iso_channel(camera,&val) == DC1394_SUCCESS
            && dc1394_iso_release_channel(camera,val) == DC1394_SUCCESS)
        {
            printf("Freed ISO %d\n",val);
        }
        dc1394_video_set_transmission(camera, DC1394_OFF);
        
        //dc1394_iso_release_bandwidth(camera, 10000000);
        /*dc1394_iso_release_bandwidth(camera, INT_MAX);
        for(chan=0;chan<64;chan++)
        {
            dc1394_iso_release_channel(camera,chan);
        }*/
        dc1394_camera_free (camera);
        cam++;
    }
    while(cam<num);
    
    dc1394_free (d);

    return 0;
}


