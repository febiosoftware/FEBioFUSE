## FEBioFUSE
The FUSE library (**F**EBio **U**nified **S**olver and **E**xchange) library allows users to couple different FEBio models and run them together, exchanging data where needed. 

The models exchange data via mesh data maps. Currently, it is assumed that all models involved use the exact same mesh and data maps that exchange data are defined on the same element set.

One of the models is tagged as the primary model. This model will be the model that is specified on the FEBio command line (with the -i command line option). All the other models are identified as secondary models. 

The interaction between the models is defined via a separate input file that is described below.

### FUSE input file
The FUSE input file is an xml formatted file that defines the models involved in the coupled simulation and how data is exchanged between the models.

The root element is called `febio_coupled_solver`.

There are two main sections in the file:

* `models`: This defines the models that are involved in the coupled simulation. 
* `exchanges`: This defines the data that needs to be exchanged between models.

#### The models section

The `models` section lists the secondary models that are involved in the simulation. Each model is defined via the `model` tag. This tag takes two attributes:

* `name`: specify a name for this model to refer to later. 
* `file`: specify the relative input file name for this model. 

Note that the primary model should not be defined in this list. By default, the name of the primary model is `fem`.  To change this, you can use the `primary_model` tag. The name is set with the `name` attribute.

```xml
    <models>
        <primary_model name="chem"/>
        <model name="mech" file="mech.feb" />
    </models>
```

#### The exchanges section
This section defines how data is exchanged between different models. Each exchange is defined via an `exchange` element. Each exchange identifies a source and destination model. For a model to serve as a destination model, it must define a mesh data map to receive the data in.

The `exchange` element defines two attributes.

* `src`: the source data, evaluated on the source model. 
* `dst`: the destination data, evaluated on the destination model’s mesh data map.

In addition, this element has the following property. 

* `filter`: a 1D function that will be applied to the source data before mapping it to the destination.

```xml
<exchange src="mech.elem_data('effective stress')" dst="chem.mesh_data('k_map')">
    <filter type="scale">
        <scale>0.1</scale>
    </filter>
</exchange>
```

### Running the coupled model
To run the coupled model using the fuse library, use the following FEBio command line.

```
>febio4 -i primary.feb -task=fuse control.xml
```

Here, `primary.feb` is the name of the primary input file (this can be any FEBio input file). The `-task=fuse` option tells FEBio to use FUSE to solve the primary model. The `control.xml` is the FUSE input file (again, you can choose the name of this file).

Along with the primary model, FUSE will also run the secondary models as needed and exchange data between all models as defined by the control file.

FEBio will output the standard log and plot files for the primary model. In addition, it will create a plot file for the secondary models. The name of the plot file is derived from the secondary model’s name.

## A complete example
This example couples a primary model, named `chem` to a secondary model named `mech`. Data is passed in both directions. 

```xml
<?xml version="1.0" encoding="ISO-8859-1"?>
<febio_coupled_solver version="1.0">
    <models>
        <primary_model name="chem"/>
        <model name="mech" file="MechDiffusion.feb" />
    </models>
    <exchanges>
        <exchange src="mech.elem_data('effective stress')" dst="chem.mesh_data('k_map')">
            <filter type="scale">
                <scale>1.13e-5</scale>
            </filter>
        </exchange>
        <exchange src="chem.elem_data('sbsc1')" dst="mech.mesh_data('E_map')">
            <filter type="scale">
                <scale>1e17</scale>
            </filter>
        </exchange>
    </exchanges>
</febio_coupled_solver>
```