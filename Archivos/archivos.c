

void personalesCargarCSV(const char *path, tPersonales *transferencias, int capacidad)
{
    FILE *archivo;
    char linea[256];
    tPersonales transferencia;
    
    int campos_leidos;
  

    archivo = fopen(path, "r");
    if (archivo == NULL)
    {
        printf("Error: no se pudo abrir %s\n", path);
        return;
    }

    while (fgets(linea, sizeof(linea), archivo) != NULL)
    {
        /* saltar linea de encabezado o lineas vacias */
        if (linea[0] == '\n' || linea[0] == '\r' || linea[0] == '#')
            continue;

        campos_leidos = sscanf(linea,
            "%d;%d;%2[^;];%49[^;];%c;%f",
            &transferencia.anio, 
            &transferencia.trimestre, 
            transferencia.codigo_pais, 
            transferencia.descripcion_pais, 
            &transferencia.tipo,
            &transferencia.monto
        );

        if(campos_leidos == 16 && strcmp(miembro.email_tutor, "") != 0)
        {
            tam_mail = strlen(miembro.email_tutor);
            strncpy(miembro.email_tutor, "", tam_mail);
        }

        if (campos_leidos != 16 && campos_leidos != 17)
            continue; /* linea mal formada, no se registra en auditoria */

        miembro_validar(&miembro, proceso, indice_miembros, auditoria);
    }

    fclose(archivo);
}
