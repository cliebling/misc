
/*
    C. Liebling
    2022-05-08

    About this code snippet:
    This code demonstrates how I would go about constructing
    the requested task of:
    1. Read data in on the serial port representing an infix arithmetic expression
    2. Process the infix expression using an implementation of the shunting yard
    algorithm
    3. Send the postfix expression back to the user over the serial port
    4. Optionally allow the user to evaluate the last expression

Issues:
    This depends on some hypothetical functions, namely those to:
    1. Configure the UART
    2. Send data to the UART
    3. Receive data from the UART

    The code is somewhat brittle, as there is not robust error checking or
    input processing. Poorly formed input will cause problems.

    I simply do not have time to actually instantiate this code inside FreeRTOS
    built on Windows. I am moving house and that is occupying a tremendous amount
    of my time, hence this solution not being executable. It does, however, demonstrate
    that I understand some of the built in constucts in FreeRTOS, namely queues and
    tasks, both of which are used extensively in any RTOS. Also note that I have
    never used FreeRTOS before, so this is all completely new to me and I am sure
    there are probably steps I missed between initializing and runnint the tasks.

    That said, it covers the basics.
*/


#define MAX_BUFFER_LEN 255U
#define MAX_QUEUE_LEN 8
#define STACK_DEPTH 512

/* Hypothetical UART handle type */
static UARTInstance_t uart_instance;

static QueueHandle_t rx_queue;
static QueueHandle_t tx_queue;


/*
 * This task:
 * Checks for incoming serial data
 * Enqueues any incoming serial data for processing by
 * the infix2postfix task
 * Checks for outgoing data to send back to the serial port
 * Sends back the queued output from the serial port
 */
void console_interface_task(void *parameters)
{
   char rx_char;
   char buffer[MAX_BUFFER_LEN];
   char tx_msg[MAX_BUFFER_LEN];
   int32_t buffer_index;
   int32_t msg_len;


   buffer_index = 0;
   memset((void*)buffer, 0, MAX_BUFFER_LEN);

   while(1)
    {
        if(xQueueReceive(tx_queue, (void*)tx_msg, 0) == pdTRUE)
        {
            /* data available to send */
            msg_len = strlen(tx_msg);
            /* Invoke this hypothetical serial transmit function */
            UARTSendString(uart_instance, tx_msg, msg_len);
        }
        /* hypothetical serial buffer check */
        if(UARTDataAvailable(uart_instance) > 0)
        {
            /* hypothetical function that gets a byte from the UART's RX buffer */
            rx_char = UARTGetByte(uart_instance);
            if(buffer_index < MAX_BUFFER_LEN - 1) /* need room for the null */
            {
                buffer[buffer_index] = rx_char;
                buffer_index++;
            }
            if(rx_char == '\n')
            {
                /* wait up to 100 system ticks for space? */
                xQueueSend(rx_queue, (void*)buffer, 100);
                /* data is copied into the queue so clear the buffer */
                memset((void*)buffer, 0, MAX_BUFFER_LEN);
            }
        }
    }
}

void process_infix_task(void *parameters)
{

    char tx_msg[MAX_BUFFER_LEN];
    char rx_msg[MAX_BUFFER_LEN];
    char prev_tx_msg[MAX_BUFFER_LEN];
    int32_t msg_len;

    memset((void*)tx_msg, 0, MAX_BUFFER_LEN);
    memset((void*)rx_msg, 0, MAX_BUFFER_LEN);
    memset((void*)prev_tx_msg, 0, MAX_BUFFER_LEN);
    while(1)
    {
        if(xQueueReceive(rx_queue, (void*)rx_msg, 0) == pdTRUE)
        {
            /* data available to send */
            msg_len = strlen(tx_msg);
            /* Evaluate the postfix on "eval" but only if the string
             * isn't empty
             */
            if((strncmp(rx_msg, "eval") == 0) && (prev_tx_msg[0] != NULL))
            {
                /* evaluate the previous postfix expression we sent */
                sprintf(tx_buffer, "%s", c_postfixEvaluator(prev_tx_msg);
                xQueueSend(tx_queue, (void*)tx_buffer, 100);
            }
            else
            {
                /* otherwise process infix and send back postfix */
                c_infix2postfix(rx_msg, tx_msg, MAX_BUFFER_LENGTH);
                xQueueSend(tx_queue, (void*)tx_msg, 100);
            }
            /* 
             * move current message to previous buffer so the "eval"
             * command will work 
             */
            memset((void*)prev_tx_msg, 0, MAX_BUFFER_LEN);
            strncpy(prev_tx_msg, tx_msg, MAX_BUFFER_LEN); 
            memset((void*)tx_msg, 0, MAX_BUFFER_LEN);
            memset((void*)rx_msg, 0, MAX_BUFFER_LEN);
        }
    }
}


int main(void)
{
    /* set up the serial port using a hypothetical serial port config 
     * first parameter is a pointer to the handle
     * second parameter is the baud rate
     * third parameter is the UART device number
     */
    UARTConfig(&uart_instance, 115200, 0);

    /* initialize the queues */
    tx_queue = xQueueCreate(MAX_QUEUE_LEN, MAX_BUFFER_LEN);
    rx_queue = xQueueCreate(MAX_QUEUE_LEN, MAX_BUFFER_LEN);

    /* initialize tasks */
    xTaskCreate(console_interface_task, "Console", STACK_DEPTH, NULL, 0, NULL);
    xTaskCreate(process_infix_task, "Console", STACK_DEPTH, NULL, 0, NULL);
    vStartTaskScheduler();
    return 0;
}
