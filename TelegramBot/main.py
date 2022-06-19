import telebot
import mysql.connector

bot = telebot.TeleBot("{}")


@bot.message_handler(commands=['start', 'help'])
def send_welcome(message_hello):
    bot.reply_to(message_hello, "Hello! I am smart flower pot. How can I help you?")


@bot.message_handler(commands=['history'])
def show_data(request):
    try:
        connection = mysql.connector.connect(
            host="localhost",
            database="micro_project",
            user="root",
            password="rootpass"
        )
        if connection.is_connected():
            db_info = connection.get_server_info()
            print("Connected to MySQL Server version ", db_info)
            query = "SELECT * FROM history"
            cursor = connection.cursor()
            cursor.execute(query)
            records = cursor.fetchall()
            print(records)
            for record in records:
                bot.reply_to(request, """
                Time of measurement: {}:{}:{}; 
Temperature: {} C; 
Humidity: {} %;
                """
                             .format(record[3], record[4], record[5], record[1], record[2]))
            cursor.close()
        connection.close()
    except mysql.connector.Error:
        print('Error while connection to MySql')


bot.infinity_polling()
