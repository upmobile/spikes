/*
file
bb_smscconn.c,

dir   /gateway-1.4.3/gw/ 


// search for this method
*/
void bb_smscconn_send_failed(SMSCConn *conn, Msg *sms, int reason, Octstr *reply)
{
    /* do not resend anything !!! Changed Jose Escobar 2012-08-09  <<<<<<<<<<<<<<<<<
    if (sms->sms.split_parts != NULL) {
        handle_split(conn, sms, reason);
        octstr_destroy(reply);
        return;
    } */

switch (reason) {
    /* do not resend anything !!! Changed Jose Escobar 2012-08-09   <<<<<<<<<<<<<<<<<<<<<<
    case SMSCCONN_FAILED_TEMPORARILY:
        *
         * Check if SMSC link alive and if so increase resend_try and set resend_time.
         * If SMSC link is not active don't increase resend_try and don't set resend_time
         * because we don't want to delay messages because of connection broken.
         *
       if (conn && smscconn_status(conn) == SMSCCONN_ACTIVE) {
            *
             * Check if sms_resend_retry set and this msg has exceeded a limit also
             * honor "single shot" with sms_resend_retry set to zero.
             *
           if (sms_resend_retry >= 0 && sms->sms.resend_try >= sms_resend_retry) {
               warning(0, "Maximum retries for message exceeded, discarding it!");
               bb_smscconn_send_failed(NULL, sms, SMSCCONN_FAILED_DISCARDED,
                                       octstr_create("Retries Exceeded"));
               break;
           }
           sms->sms.resend_try = (sms->sms.resend_try > 0 ? sms->sms.resend_try + 1 : 1);
           time(&sms->sms.resend_time);
       }
       gwlist_produce(outgoing_sms, sms);
       break;

    case SMSCCONN_FAILED_SHUTDOWN:
        gwlist_produce(outgoing_sms, sms);
        break;
    */
    /* Use only the default NACK!!! do not resend anything Changed Jose Escobar 2012-08-09 */
    default:
default:
        /* write NACK to store file */
        store_save_ack(sms, ack_failed);

        if (conn) counter_increase(conn->failed);
        if (reason == SMSCCONN_FAILED_DISCARDED)
            bb_alog_sms(conn, sms, "DISCARDED SMS");
        else
            bb_alog_sms(conn, sms, "FAILED Send SMS");

        /* generate relay confirmancy message */
        if (DLR_IS_SMSC_FAIL(sms->sms.dlr_mask) ||
            DLR_IS_FAIL(sms->sms.dlr_mask)) {
            Msg *dlrmsg;

            if (reply == NULL)
                reply = octstr_create("");

            octstr_insert_data(reply, 0, "NACK/", 5);
            dlrmsg = create_dlr_from_msg((conn ? (conn->id?conn->id:conn->name) : NULL), sms,
                                         reply, DLR_SMSC_FAIL);
            if (dlrmsg != NULL) {
                bb_smscconn_receive(conn, dlrmsg);
            }
        }

        msg_destroy(sms);
        break;
    }

    octstr_destroy(reply);
}


