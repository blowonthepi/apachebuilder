#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

using namespace std;

void askDomain();
void askIncludeWwwPrefix();
void askAdminEmail();
void confirmUserInputs();
void restart();

string domain, email;

bool includeWWWPrefix;

void handleBuild() {
    string filename = domain;
    std::replace(filename.begin(), filename.end(), '.', '-');
    ofstream file("/etc/apache2/sites-available/" + domain + ".conf");

    file << "<VirtualHost *:80>" << endl;
    file << "ServerAdmin " << email << endl;
    file << "ServerName " << domain << endl;
    file << "ServerAlias www." << domain << endl;
    file << "DocumentRoot /var/www/html/" << domain << endl;
    file << "ErrorLog ${APACHE_LOG_DIR}/error.log" << endl;
    file << "CustomLog ${APACHE_LOG_DIR}/access.log combined" << endl;
    file << "</VirtualHost>" << endl;
    file << "<Directory /var/www/html/" << domain << ">" << endl;
    file << "Options Indexes FollowSymLinks\n"
            "AllowOverride All\n"
            "Require all granted" << endl;
    file << "</Directory>" << endl;

    system("a2ensite");
    string cmd = "sudo certbot --apache --agree-tos --email " + email + " --hsts -d " + domain;
    system(cmd.c_str());
}

void getDomain() {
    askDomain();

    askIncludeWwwPrefix();

    askAdminEmail();

    confirmUserInputs();
}

/**
 * @brief Ask the user which domain they'd like to setup with Apache
 * 
 */
void askDomain() {
    cout << "** PLEASE TYPE THE DOMAIN **" << endl;
    cout << "* (without http(s)://www.) *" << endl;
    cout << "Domain: ";
    cin >> domain;
}

/**
 * @brief Ask the user whether or not they want an automatically generated "www." alias.
 * 
*/
void askIncludeWwwPrefix() {
    string prefixQ;
    cout << "Would you like to include the \"www.\" prefix also? (y/n): ";
    cin >> prefixQ;

    if (prefixQ == "y" || prefixQ == "Y") {
        includeWWWPrefix = true;
    } else if (prefixQ == "n" || prefixQ == "N") {
        includeWWWPrefix = false;
    } else {
        askIncludeWwwPrefix();
    }
}

/**
 * @brief Ask the user what email they want for admin purposes.
 * 
 */
void askAdminEmail() {
    cout << "** PLEASE TYPE THE ADMIN EMAIL **" << endl;
    cout << "Email: ";
    cin >> email;
}

/**
 * @brief Ask that the user confirms the request they're making
 * 
 */
void confirmUserInputs() {
    cout << "Inputs that will be used:" << endl;
    cout << "* Domain: " << domain << endl;
    cout << "* Include WWW Prefix?: " << (includeWWWPrefix ? "Yes" : "No") << endl;
    cout << "* Admin Email: " << email << endl;

    cout << "Continue? (y/n/c[cancel]): ";
    string confirm;
    cin >> confirm;

    if (confirm == "y") {
        handleBuild();
    } else if (confirm == "n") {
        restart();
    } else {
        exit(0);
    }
}

void restart() {
    system("./main");
    exit(0);
}

int main() {
    cout << "******************************************" << endl;
    cout << "** WELCOME TO THE APACHE CONFIG CREATOR **" << endl;

    getDomain();

    return 0;
}
