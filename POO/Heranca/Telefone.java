class Telefone{
    private int codigo;
    private String modelo, numSerie;

    public Telefone(int codigo, String modelo, String numSerie)
    {
        this.codigo = codigo;
        this.modelo = modelo;
        this.numSerie = numSerie;
    }

    public void imprimirDados()
    {
        System.out.println("Código: " + this.codigo);
        System.out.println("Modelo: " + this.modelo);
        System.out.println("Número de Série : " + this.numSerie);
    }

    
}